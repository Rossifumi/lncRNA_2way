#!/user/bin/perl
#usage: perl simp_chr_name_rice.pl $input_genome $output_genome

open(GNOM_IN, "<$ARGV[0]") or die $!;
open(GNOM_OUT, ">$ARGV[1]") or die $!;

while(<GNOM_IN>)
{
	chomp $_; $line = $_;
	if ($line =~ /^>Chr(\S+)/)
	{
		$name = $1;
		print OUT ">$name\n";
	}
	else
	{
		print OUT "$line\n";
	}
}
