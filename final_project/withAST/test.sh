for file in ./testdata/*
do
    echo "Testing $file"
    ./Mini-LISP < $file
    echo "-----------------------------------"
done
