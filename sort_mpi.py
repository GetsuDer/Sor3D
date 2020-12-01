f = open("mpi_64", "r").readlines()
out = open("mpi_res", "w")
for i in range(len(f)):
    j = f[i].split()
    f[i] = [int(j[1]), j[2]]
f.sort()
for i in f:
    out.write(str(i[0]) + " " + i[1] + "\n")
    
