for N in range(204, 2004, 10):
    for it in range(1, 161):
        out = open("planner_" + str(it) + "_" + str(N), "w");
        out.write("#BSUB -n " + str((it + 7) // 8))
        out.write("\n#BSUB -W 00:20\n#BSUB -o ")
        out.write("Output/%J\n#BSUB -e Res/%J\n#BSUB -R")
        out.write("\"span[hosts=1]\"\nOMP_NUM_THREADS=1 ./parallel" + str(N) + " " + str(it))
        out.close()
