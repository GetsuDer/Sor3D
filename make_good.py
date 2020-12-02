f = open("res_64", 'r').readlines()
out = open("build_graph", 'w')
for i in f:
    out.write(i)
    out.write("\n")
