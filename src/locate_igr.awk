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
	if (end1[$1]!=0 && start1[$2]!=0 && end2[$5]!=0 && start2[$6]!=0)
	    print $1"\t"$2"\t"$3"\t"$4"\t"end1[$1]+1"\t"start1[$2]-1"\t"$5"\t"$6"\t"$7"\t"$8"\t"end2[$5]+1"\t"start2[$6]-1
    }

}

END {
}
