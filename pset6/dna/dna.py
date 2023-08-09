# Open csv file and dna sequence, and read contents
import sys
import csv

def main():
    if len(sys.argv) != 3:
        sys.exit("Missing command-line argument")

    rows = []
    samples = []
    with open(sys.argv[1], "r") as names_file:
        reader = csv.DictReader(names_file)
        samples = reader.fieldnames[1:]
        for row in reader:
            rows.append(row)

    repeat = []
    # Read dna sequence and find max repeated STR
    dna = (open(sys.argv[2], "r")).read()
    for i in range (len(samples)):
        sample = samples[i]
        max_repeat = str_repeat(dna, sample)
        repeat.append(max_repeat)

    for row in rows:
        lst = []
        for sample in samples:
            lst.append(int(row[sample]))
        if repeat == lst:
            print(row['name'])
            sys.exit(0)

    print("No match")

# Look at all to STR and find which one is the longest
def str_repeat(dna, sample):
    count_list = []
    count = 1
    match = 0
    ln = len(sample)
    d_ln = len(dna)

    for i in range(d_ln - ln):
        if dna[i : i + ln] == sample:

            if i + (ln * 2) == d_ln and dna[i + ln : i + (ln * 2)] == sample:
                count += 1
                count_list.append(count)

            if dna[i : i + ln] == dna[i + ln : i + (ln * 2)]:
                count += 1
                match += 1

            else:
                count_list.append(count)
                count = 1

    if match == 0:
        count = 0

    if not count_list:
        max_repeat = 0
    else:
        max_repeat = max(count_list)
    return max_repeat

main()