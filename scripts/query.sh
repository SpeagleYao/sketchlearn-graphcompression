for k in 2 3 4 5 6 7 8 9 10
do 
  for m in 10, 30, 50, 70, 90, 110, 130, 150, 170, 200
  do
    echo $k $m
    ./build/main -k $k -f data/test2/test.prov -m $m
  done
done