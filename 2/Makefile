sequencial:
	mpicc src/sequencial.c -o out/sequencial -fopenmp
	sbatch batchjobs/sequencial.batchjob

um-nodo:
	mpicc src/paralelo.c -o out/paralelo -fopenmp
	for batchjob in batchjobs/1-node/*.batchjob ; do \
		sbatch $$batchjob ; \
	done

dois-nodos:
	mpicc src/paralelo.c -o out/paralelo -fopenmp
	for batchjob in batchjobs/2-nodes/*.batchjob ; do \
		sbatch $$batchjob ; \
	done

clean:
	rm -rf out/*
	rm -rf data/sequencial.txt
	rm -rf data/1-node/*
	rm -rf data/2-nodes/*
