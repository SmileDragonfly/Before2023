# cluster.py
def readfile(filename):
    lines = [line for line in file(filename)]
    # first line is the column tittles
    colnames = lines[0].strip('\t')[1:]
    # bo qua chuong 2