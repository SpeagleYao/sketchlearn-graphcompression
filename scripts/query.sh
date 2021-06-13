# LOG_FILE="./data/sample10_new_10rounds/sample10_new_10rounds_mutualtrust1-2.log"
# SKETCH_FILE="./data/sample10_new_10rounds/sample_10_new_10rounds.prov"
# PROB_FILE="./data/sample10_new_10rounds/sample_10_new.csv"
# PROV_FILE="./data/sample10_new_10rounds/mutualtrust1-2.txt"
LOG_FILE="./data/prob_test5/sample5_10rounds_mutualtrust1-2.log"
SKETCH_FILE="./data/prob_test5/sample5_10rounds.prov"
PROB_FILE="./data/prob_test5/sample5.csv" # ?
PROV_FILE="./data/prob_test5/mutualtrust1-2.txt" # ?
rm $LOG_FILE
for p in 0.6 0.7 0.8 0.9 1.0
do 
  for (( c=1; c<=10; c++ ))
  do
    echo $p >> $LOG_FILE
    ./build/main -fs $SKETCH_FILE -fd $PROB_FILE -fp $PROV_FILE -q mutualTrustPath1-2 -p $p >> $LOG_FILE
  done
done