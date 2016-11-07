#seqid: Chr1
#source: MSU_osa1r6
#type: gene
#start: 10218
#end: 11435
#score: .
#strand: +
#phase: . 
#attributes: ID=13101.t00002;Name=expressed%20protein;Alias=LOC_Os01g01019

BEGIN {
    FS = "\t"
    OFS = "\t"
}

# ignore lines that are not mRNA
NF == 9 && $3 != "mRNA" {
    next
}

# load data from gff file
NF == 9 {
    seqid = $1
    source = $2
    type = $3
    start = $4
    end = $5
    score = $6
    strand = $7
    phase = $8
    delete val
    split($9, tokens, ";")
    for (i in tokens) {
        split(tokens[i], parts, "=")
        val[parts[1]] = parts[2]
    }
    if (val["Name"] != "") {
        header[val["Name"]] = $1 "|" $2 "|" $3 "|" $4 "|" $5 "|" $6 "|" $7 "|" $8
    }
    next
}

# augment header in pep file with data from gff
/^>/ {
    split($0,tok,"[>|]")
    id = tok[2]
    print $0 "|" header[id]
    next
}

/^[A-Z]/ {
    print
}
