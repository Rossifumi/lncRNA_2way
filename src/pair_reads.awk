BEGIN {
    mate=1
}

{
    if (mate==1) {
	start=$4
	mate=2
    }
    else {
	split($1,a,"/")
	read=a[1]
	region=$3
	end=$4+length($5)-1
	print read "\t" region "\t" start "\t" end
	mate=1
    }
}

END {
}
