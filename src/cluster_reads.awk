BEGIN {
    region=""
}

{
    if ($2!=region) {
	if (NR>1)
	    print region "\t" start "\t" end "\t" count "\t" countR "\t" end-start+1 "\t" gap
	region=$2
	start=$3
	end=$4
	gap=0
	count=1
	countR=1
    }
    else {
	if ($3-end-1>=100) {
	    print region "\t" start "\t" end "\t" count "\t" countR "\t" end-start+1 "\t" gap
	    start=$3
	    end=$4
	    gap=0
	    count=1
	    countR=1
	}
	else {
	    if ($3>end+1)
		gap+=$3-end-1
	    if ($4>end) {
		end=$4
		countR++
	    }
	    count++
	}
    }
}

END {
    print region "\t" start "\t" end "\t" count "\t" countR "\t" end-start+1 "\t" gap
}
