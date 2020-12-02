for name in ['20', '40', '64', '74', '84', '100', '114', '128']:
    f = open("res_" + name, "r").readlines()
    out = open("mpi_res", "a")
    for i in f:
        out.write(i + "\n")
    
