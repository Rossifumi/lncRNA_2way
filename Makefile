SHELL=/bin/bash

bin: bin/extract_blast_out_100 bin/ordCons-genePair bin/blast_cons_rgn bin/fwd_rev \
	bin/blast_filterE bin/extract_cand_fwd_wrap bin/integrate_2way bin/rnaalifold_aln_rgn \
	bin/clustalw_aln_rgn bin/extract_igr_fwd bin/oneline_genome bin/rnaz_aln_rgn

bin/%: src/%.cpp
	g++ -o $@ $^

# pre-process genomes to pep and info files (should be available after BBH-LS)

%_allpep.fa: %_peptide.fa %_gene.gff3
	awk -f src/gff3pep2pep.awk $(word 2, $^) <(sed -e 's/|.*$$//g' $(word 1, $^)) > $@

%.info: %_allpep.fa %_split.awk
	cat $(word 1, $^) | tr -d '*' | awk -f $(word 2, $^) > $(@:info=pep) 2> $@

# sort the peptides

%.ord: %.info
	sort -nk 5,6 $^ | sort -sk 3,4 | awk -f src/info2ord.awk > $@

# 2-way conserved gene pairs
# case specific

%ZM-OS_ORDCONS_GNP.tsv: %Zmays_181.ord %Osativa_204.ord %ZM-OS_BBH-LS.spairs
	 bin/ordCons-genePair $^ > $@


# annotate gene pairs with chromosome number
# case specific

%ZM-OS_ORDCONS_GNP_annotated.tsv: %Zmays_181.info %Osativa_204.info %ZM-OS_ORDCONS_GNP.tsv
	awk -f src/annotate_GNP.awk $^ > $@


# add more location information to gene pairs
# case specific

%ZM-OS_ORDCONS_GNP_fullinfoP.tsv: %Zmays_181.info %Osativa_204.info %ZM-OS_ORDCONS_GNP_annotated.tsv
	awk -f src/locate_igr.awk $^ > $@


# covert each chromesome into one line
%_oneline.fa: %.fa
	bin/oneline_genome $^ $@


#simplify the chromsome name
%_oneline_simp.fa: %_oneline.fa
	perl src/simp_chr_name_rice.pl $^ $@


# extract the syntenically conserved intergenic regions
# case specific
%ZM_ZM-OS_ORDCONS_IGR_fwd.fa: %Zmays_181_oneline.fa %Osativa_204_oneline.fa %ZM-OS_ORDCONS_GNP_fullinfoP.tsv
	perl src/fetch_target_genomic_region.pl $^ $@ OS_ZM-OS_ORDCONS_IGR_fwd.fa


# convert into sense strand

%_ORDCONS_IGR.fa: %_ORDCONS_IGR_fwd.fa
	bin/fwd_rev $^ $@

# build bowtie index file

%_index: %.fa
	bowtie-build $^ $@
	touch $@

# use bowtie to map RNA-Seq reads to intergenic regions
# case specific

test/ZM_ZM-OS_maize-test_fwd.map: test/ZM_ZM-OS_ORDCONS_IGR_fwd_index test/maize-test_1.fq test/maize-test_2.fq
	bowtie $(word 1, $^) -1 $(word 2, $^) -2 $(word 3, $^) $@


# retain the location info

%_MAPPED.tsv: %.map
	awk -f src/pair_reads.awk $^ > $@

# sort the mapped reads according to location

%_SORTED.tsv: %_MAPPED.tsv
	sort -nk 3,4 $^ | sort -sk 2,2 > $@

# assemble the reads into candidates

%_ASSEMBLED.tsv: %_SORTED.tsv
	awk -f src/cluster_reads.awk $^ > $@

# extract sequences from intergenic regions
# case specific

%_maize-test_cand_fwd_wrap.fa: %_ORDCONS_IGR_fwd.fa %_maize-test_fwd_ASSEMBLED.tsv
	bin/extract_cand_fwd_wrap $^ $@

# convert into sense strand

%_cand_wrap.fa: %_cand_fwd_wrap.fa
	bin/fwd_rev $^ $@

# blast against intergenic regions
# case specific

test/ZM_ZM-OS_maize-test_cand_wrap_BLAST.out: test/ZM_ZM-OS_ORDCONS_IGR.fa test/OS_ZM-OS_ORDCONS_IGR.fa test/ZM_ZM-OS_maize-test_cand_wrap.fa
	bin/blast_cons_rgn $^ $@


# filter with E-value (1e-6)

%_BLAST-P6.out: %_BLAST.out
	bin/blast_filterE $^ 1e-6 $@

# assemble the blast hits to homolog candidates
# case specific

test/ZM_ZM-OS_maize-test_cand_wrap_BLAST-P6.fa: test/ZM_ZM-OS_ORDCONS_IGR.fa test/OS_ZM-OS_ORDCONS_IGR.fa test/ZM_ZM-OS_maize-test_cand_wrap_BLAST-P6.out
	bin/extract_blast_out_100 $^ $@


# generate the 2-way conserved homologs
# case specific

test/ZM_ZM-OS_maize-test_cand_wrap_2way.fa: test/ZM_ZM-OS_maize-test_cand_wrap.fa test/ZM_ZM-OS_maize-test_cand_wrap_BLAST-P6.fa
	 bin/integrate_2way $^ $@ test/OS_ZM-OS_maize-test_cand_wrap_2way.fa


# use clustalw to generate multiple alignment
# case specific

test/ZM_ZM-OS_maize-test_cand_wrap_CLUSTALW.out: test/ZM_ZM-OS_maize-test_cand_wrap_2way.fa test/OS_ZM-OS_maize-test_cand_wrap_2way.fa
	bin/clustalw_aln_rgn $^ $@

# use RNAz to evalate ncRNA potential
# case specific

test/ZM_ZM-OS_maize-test_cand_wrap_RNAZ.out: test/ZM_ZM-OS_maize-test_cand_wrap_2way.fa test/ZM_ZM-OS_maize-test_cand_wrap_CLUSTALW.out
	bin/rnaz_aln_rgn $^ $@


# use rnaalifold to generate secondarty structure
# case specific

test/ZM_ZM-OS_maize-test_cand_wrap_RNAALIFOLD.out: test/ZM_ZM-OS_maize-test_cand_wrap_2way.fa test/ZM_ZM-OS_maize-test_cand_wrap_CLUSTALW.out
	bin/rnaalifold_aln_rgn $^ $@ test/ZM_ZM-OS_maize-test_cand_wrap

