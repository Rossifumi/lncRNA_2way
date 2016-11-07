BEGIN {
    c=0
}

{
    if (ARGIND==1) {
	gene = $1
	chr1[gene] = gensub("^chr","","g",$3)
	strand1[gene] = $4
	start1[gene] = $5
	end1[gene] = $6
    }
    else if (ARGIND==2) {
	gene = $1
	chr2[gene] = gensub("^chr","","g",$3)
	strand2[gene] = $4
	start2[gene] = $5
	end2[gene] = $6
    }
    else if (ARGIND==3) {
	print $1"\t"$2"\t"chr1[$1]"\t"strand1[$1]"\t"$3"\t"$4"\t"chr2[$3]"\t"strand2[$3]
    }

}

END {
}
