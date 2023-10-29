LD_LIBRARY_PATH=../../../binaries/linux/x86_64:$LD_LIBRARY_PATH ./benchmark \
    --image "../../../assets/images/United States - California Driving License (2017).jpg" \
    --assets ../../../assets \
    --loops 10 \
    --gpu_ctrl_mem false \
    --vino_activation "auto" \
    --parallel true