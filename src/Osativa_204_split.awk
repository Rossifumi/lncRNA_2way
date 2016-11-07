# for each gene, select the longest transcript (based on peptide length)
# if there are multiple longest transcript, use the one whose transcript id is lexicographically smallest
# for each gene, print gene id, chr, start, end

BEGIN {
    seq = ""
}

function process_gene()
{
    if (seq == "" || seq ~ /avail/) return

    seqs[tid] = seq
    len[tid] = length(seq)
    
    if (!(gid in rep)) {
        rep[gid] = tid
        return
    }
    
    curr = rep[gid]

    if (len[tid] > len[curr]) {
        rep[gid] = tid
        return
    } else if (len[tid] < len[curr]) {
        return
    }
    # len[tid] == len[curr]

    if (start[tid] < start[curr]) {
        rep[gid] = tid
        return
    } else if (start[tid] > start[curr]) {
        return
    }
    # start[tid] == start[curr]

    if (end[tid] > end[curr]) {
        rep[gid] = tid
        return
    } else if (end[tid] < end[curr]) {
        return
    }
    # end[tid] == end[curr]
   
    # doesn't matter since seqs are also identical
    #if (seqs[tid] != seqs[curr]) {
    #    print "TIE BREAKING FAILED", tid, curr
    #}

    # tid is unique, not possible to have tid == curr
    if (tid < curr) {
        rep[gid] = tid
        return
    } else if (tid > curr) {
        return
    }
}

# >ATCG00500.1|ChrC|phytozome7_0|mRNA|57075|58541|.|+|.
# MEKSWFNFMFSKGELEYRGELSKAMDSFAPGEKTTISQDRFIYDMDKNFYGWDERSSYSSSYSNNVDLLVSSKDIRNFIS
# DDTFFVRDSNKNSYSIFFDKKKKIFEIDNDFSDLEKFFYSYCSSSYLNNRSKGDNDLHYDPYIKDTKYNCTNHINSCIDS
# YFRSYICIDNNFLIDSNNFNESYIYNFICSESGKIRESKNYKIRTNRNRSNLISSKDFDITQNYNQLWIQCDNCYGLMYK
# KVKMNVCEQCGHYLKMSSSERIELSIDPGTWNPMDEDMVSADPIKFHSKEEPYKNRIDSAQKTTGLTDAVQTGTGQLNGI
# PVALGVMDFRFMGGSMGSVVGEKITRLIEYATNQCLPLILVCSSGGARMQEGSLSLMQMAKISSVLCDYQSSKKLFYISI
# LTSPTTGGVTASFGMLGDIIIAEPYAYIAFAGKRVIEQTLKKAVPEGSQAAESLLRKGLLDAIVPRNLLKGVLSELFQLH
# AFFPLNTN

/^>/ {
    process_gene()
    
    split($0,tok,"[>|]")
    tid = tok[2]
    gid = gensub(".[0-9]+$", "", "g", tid)
    chr[tid] = gensub("Chr", "" ,"g", tok[3])
    start[tid] = tok[6]
    end[tid] = tok[7]
    strand[tid] = tok[9]

    if (0) {
        print "gid = " gid 
        print "tid = " tid 
        print "char = " chr[tid]
        print "start = " start[tid]
        print "end = " end[tid]
        print "strand = " strand[tid]
        print "length = " len[tid]
    }
    
    seq = ""
    next
}

{
    seq = seq $0
}

END {
    process_gene()

    OFS = "\t"
    asorti(rep, genes)
    for (i in genes) {
        gid = genes[i]
        tid = rep[gid]
        seq = seqs[tid]

        dir = strand[tid]

        if (chr[tid] ~ /scaff/) {
            chr_str = "chr1"
        } else {
            chr_str = "chr"chr[tid]
        }
        
        print ">" gid
        print seq
        print gid, tid, chr_str, dir, start[tid], end[tid] > "/dev/stderr"
    }
}
