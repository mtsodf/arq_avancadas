
#!/bin/sh
 
### Declare myprogram non-rerunable
#PBS -r n 
 
#PBS -e opt2.log.error
#PBS -o opt2.log.output
#PBS -q bigmem
#PBS -l nodes=6:ppn=16
 
source /nethome/drms_ci/GeomecBR_CI/SCRIPTS/geomecbr.sh

cd ~/arq_avancadas-master/projeto_final/build/tests

cat /proc/cpuinfo


caso="../../ALL_tsp/pr2392.tsp"
solucao="../../ALL_tsp/pr2392.opt.tour"

caso="../../ALL_tsp/rl5915.tsp"
solucao=""


caso="../../ALL_tsp/d15112.tsp"
solucao=""


out="resultados_6_nos_random.txt"

for i in 1 2 3 4
do
for np in 16 32 64 96
do
echo ""
echo "******************************************************************"
echo "MPI np=$np Caso=${caso}"  
echo "******************************************************************"

mpirun -np ${np} ./Opt2MPITest ${caso} ${out} ${solucao} 

done

done



