seq:
	gcc -o out/sequencial src/bubble_sort_seq.c -fopenmp
	for batchjob in batchjobs/seq/*.batchjob ; do \
		sbatch $$batchjob ; \
	done

par:
	gcc -o out/paralelo src/bubble_sort_par.c -fopenmp	
	for batchjob in batchjobs/par/$(THREADS)/*.batchjob ; do \
		sbatch $$batchjob ; \
	done

clean:
	rm -rf out/*
	rm -rf data/seq/*
	rm -rf data/par/02/*
	rm -rf data/par/04/*
	rm -rf data/par/08/*
	rm -rf data/par/16/*
