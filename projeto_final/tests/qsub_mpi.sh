
#!/bin/sh

### Declare myprogram non-rerunable
#PBS -r n

#PBS -e geomec.log.error
#PBS -o geomec.log.output
#PBS -q bigmem
#PBS -l nodes=1:ppn=16


cd ~/arq_avancadas-master/projeto_final/build/tests

caso="../../ALL_tsp/att48.tsp"
solucao="../../ALL_tsp/att48.opt.tour"
out="resultados_random_umno.txt"


for i in 1 2 3 4 5 6 7 8 9 10
do
for np in 1 2 4 8 16
do
echo ""
echo "******************************************************************"
echo "MPI np=$np Caso=${caso}"
echo "******************************************************************"

mpirun -np ${np} ./Opt2MPITest ${caso} ${out} ${solucao}



echo ""
echo "******************************************************************"
echo "OpenMP np=$np Caso=${caso}"
echo "******************************************************************"

export OMP_NUM_THREADS=${np}
mpirun -np 1 ./Opt2OpenMPTest ${caso} ${out} ${solucao}

done

done



caso="../../ALL_tsp/rd100.tsp"
solucao="../../ALL_tsp/rd100.opt.tour"

for i in 1 2 3 4 5 6 7 8 9 10
do
for np in 1 2 4 8 16
do
echo ""
echo "******************************************************************"
echo "MPI np=$np Caso=${caso}"
echo "******************************************************************"

mpirun -np ${np} ./Opt2MPITest ${caso} ${out} ${solucao}


echo ""
echo "******************************************************************"
echo "OpenMP np=$np Caso=${caso}"
echo "******************************************************************"

export OMP_NUM_THREADS=${np}
mpirun -np 1 ./Opt2OpenMPTest ${caso} ${out} ${solucao}

done

done


caso="../../ALL_tsp/pr1002.tsp"
solucao="../../ALL_tsp/pr1002.opt.tour"

for i in 1 2 3 4 5 6 7 8 9 10
do
for np in 1 2 4 8 16
do
echo ""
echo "******************************************************************"
echo "MPI np=$np Caso=${caso}"
echo "******************************************************************"

mpirun -np ${np} ./Opt2MPITest ${caso} ${out} ${solucao}


echo ""
echo "******************************************************************"
echo "OpenMP np=$np Caso=${caso}"
echo "******************************************************************"

export OMP_NUM_THREADS=${np}
mpirun -np 1 ./Opt2OpenMPTest ${caso} ${out} ${solucao}

done

done


caso="../../ALL_tsp/pcb442.tsp"
solucao="../../ALL_tsp/pcb442.opt.tour"

for i in 1 2 3 4 5 6 7 8 9 10
do
for np in 1 2 4 8 16
do
echo ""
echo "******************************************************************"
echo "MPI np=$np Caso=${caso}"
echo "******************************************************************"

mpirun -np ${np} ./Opt2MPITest ${caso} ${out} ${solucao}


echo ""
echo "******************************************************************"
echo "OpenMP np=$np Caso=${caso}"
echo "******************************************************************"

export OMP_NUM_THREADS=${np}
mpirun -np 1 ./Opt2OpenMPTest ${caso} ${out} ${solucao}

done

done


caso="../../ALL_tsp/rl5915.tsp"
solucao=""

for i in 1 2
do
for np in 4 8 16
do
echo ""
echo "******************************************************************"
echo "MPI np=$np Caso=${caso}"
echo "******************************************************************"

mpirun -np ${np} ./Opt2MPITest ${caso} ${out} ${solucao}


echo ""
echo "******************************************************************"
echo "OpenMP np=$np Caso=${caso}"
echo "******************************************************************"

export OMP_NUM_THREADS=${np}
mpirun -np 1 ./Opt2OpenMPTest ${caso} ${out} ${solucao}

done

done



