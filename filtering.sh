for filename in "G.pgm" "H.pgm"
do
    # 文字列の取得
    sublength=`expr ${#filename} - 4`
    file=`echo $filename | cut -c -$sublength`

    # 縦微分フィルタ
    echo "3 3" > mat.dat
    echo "0.0 -1.0 0.0" >> mat.dat
    echo "0.0 0.0 0.0" >> mat.dat
    echo "0.0 1.0 0.0" >> mat.dat
    ./filtering $filename mat.dat
    mv ${file}_filter.pgm ${file}_vert_diff.pgm

    # 横微分フィルタ
    echo "3 3" > mat.dat
    echo "0.0 0.0 0.0" >> mat.dat
    echo "-1.0 0.0 1.0" >> mat.dat
    echo "0.0 0.0 0.0" >> mat.dat
    ./filtering $filename mat.dat
    mv ${file}_filter.pgm ${file}_hori_diff.pgm

    # 縦のプリューウィットフィルタ
    echo "3 3" > mat.dat
    echo "-1.0 -1.0 -1.0" >> mat.dat
    echo "0.0 0.0 0.0" >> mat.dat
    echo "1.0 1.0 1.0" >> mat.dat
    ./filtering $filename mat.dat
    mv ${file}_filter.pgm ${file}_vert_prewitt.pgm

    # 横のプリューウィットフィルタ
    echo "3 3" > mat.dat
    echo "-1.0 0.0 1.0" >> mat.dat
    echo "-1.0 0.0 1.0" >> mat.dat
    echo "-1.0 0.0 1.0" >> mat.dat
    ./filtering $filename mat.dat
    mv ${file}_filter.pgm ${file}_hori_prewitt.pgm

    # 縦のソーベルフィルタ
    echo "3 3" > mat.dat
    echo "-1.0 -2.0 -1.0" >> mat.dat
    echo "0.0 0.0 0.0" >> mat.dat
    echo "1.0 2.0 1.0" >> mat.dat
    ./filtering $filename mat.dat
    mv ${file}_filter.pgm ${file}_vert_sobel.pgm

    # 横のソーベルフィルタ
    echo "3 3" > mat.dat
    echo "-1.0 0.0 1.0" >> mat.dat
    echo "-2.0 0.0 2.0" >> mat.dat
    echo "-1.0 0.0 1.0" >> mat.dat
    ./filtering $filename mat.dat
    mv ${file}_filter.pgm ${file}_hori_sobel.pgm

    # 縦2次微分フィルタ
    echo "3 3" > mat.dat
    echo "0.0 1.0 0.0" >> mat.dat
    echo "0.0 -2.0 0.0" >> mat.dat
    echo "0.0 1.0 0.0" >> mat.dat
    ./filtering $filename mat.dat
    mv ${file}_filter.pgm ${file}_vert_secondary_diff.pgm

    # 横2次微分フィルタ
    echo "3 3" > mat.dat
    echo "0.0 0.0 0.0" >> mat.dat
    echo "1.0 -2.0 1.0" >> mat.dat
    echo "0.0 0.0 0.0" >> mat.dat
    ./filtering $filename mat.dat
    mv ${file}_filter.pgm ${file}_hori_secondary_diff.pgm

    # ラプラシアンフィルタ
    echo "3 3" > mat.dat
    echo "0.0 1.0 0.0" >> mat.dat
    echo "1.0 -4.0 1.0" >> mat.dat
    echo "0.0 1.0 0.0" >> mat.dat
    ./filtering $filename mat.dat
    mv ${file}_filter.pgm ${file}_laplacian.pgm

    # 鮮鋭化処理
    echo "3 3" > mat.dat
    echo "0.0 -1.0 0.0" >> mat.dat
    echo "-1.0 5.0 -1.0" >> mat.dat
    echo "0.0 -1.0 0.0" >> mat.dat
    ./filtering $filename mat.dat
    mv ${file}_filter.pgm ${file}_laplacian.pgm

done
