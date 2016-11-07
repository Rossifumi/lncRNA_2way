#!/user/bin/perl
#usage: perl fetch_target_genomic_region.pl $input_genome_1 $input_genome_2 $input_posi_file $out_fasta_1 $out_fasta_2

open(GNOM1, "<$ARGV[0]") or die $!;
open(GNOM2, "<$ARGV[1]") or die $!;
open(POSI, "<$ARGV[2]") or die $!;

open(FA1, ">$ARGV[3]") or die $!;
open(FA2, ">$ARGV[4]") or die $!;

@chr_no_array_1=();
@chrome_1=();
%genome_index_1=();
$mark_1 = 0;

@chr_no_array_2=();
@chrome_2=();
%genome_index_2=();
$mark_2 = 0;

sub get_subseq
{
        my ($chr_seq,$chr_no,$start,$end,$strand) = @_;

        $sub_seq = substr $chr_seq, $start -1, $end - $start +1;

        if ($strand eq "-")
        {
                $sub_seq_1 = reverse $sub_seq;
                $sub_seq_1 =~ tr/ACGT/TGCA/;
                return $sub_seq_1;
        }
        else
        {return $sub_seq;}
}

while(<GNOM1>)
{
        $line = $_;
        if ($mark_1 > 1)
        {
                $chr_no_1 = shift @chr_no_array_1; $chrome_seq_1 = join('',@chrome_1);
                $genome_index_1{$chr_no_1}=$chrome_seq_1;
                $mark_1--;
                @chrome_1=();
        }
        if ($line =~ /^>(\S+).*/)
        {
                push(@chr_no_array_1,$1);
                $mark_1++;
        }
        elsif ($line =~ /^([TCGANatcgn]\w+)/)
        {
                push(@chrome_1,$1);
        }
}
$chr_no_1 = shift @chr_no_array_1; $chrome_seq_1 = join('',@chrome_1);
$genome_index_1{$chr_no_1}=$chrome_seq_1;

while(<GNOM2>)
{
        $line = $_;
        if ($mark_2 > 1)
        {
                $chr_no_2 = shift @chr_no_array_2; $chrome_seq_2 = join('',@chrome_2);
                $genome_index_2{$chr_no_2}=$chrome_seq_2;
                $mark_2--;
                @chrome_2=();
        }
        if ($line =~ /^>(\S+).*/)
        {
                push(@chr_no_array_2,$1);
                $mark_2++;
        }
        elsif ($line =~ /^([TCGANatcgn]\w+)/)
        {
                push(@chrome_2,$1);
        }
}
$chr_no_2 = shift @chr_no_array_2; $chrome_seq_2 = join('',@chrome_2);
$genome_index_2{$chr_no_2}=$chrome_seq_2;

while(<POSI>)
{
	chomp $_; $line = $_;
	if ($line =~ /^(\S+)\s+(\S+)\s+(\d+)\s+(\S+)\s+(\d+)\s+(\d+)\s+(\S+)\s+(\S+)\s+(\d+)\s+(\S+)\s+(\d+)\s+(\d+)/)
	{
		$name_1 = $1.":".$2."|".$4; $chr_no_1 = $3; $strand_1 = $4; $coor_left_1 = $5; $coor_right_1 = $6;
		$name_2 = $7.":".$8."|".$10;$chr_no_2 = $9; $strand_2 = $10;$coor_left_2 = $11;$coor_right_2 = $12;
		$fasta_1 = get_subseq($genome_index_1{$chr_no_1},$chr_no_1,$coor_left_1,$coor_right_1,$strand_1);
		$fasta_2 = get_subseq($genome_index_2{$chr_no_2},$chr_no_2,$coor_left_2,$coor_right_2,$strand_2);
		print FA1 ">$name_1\n$fasta_1\n";
		print FA2 ">$name_2\n$fasta_2\n";
	}
}
		
		
