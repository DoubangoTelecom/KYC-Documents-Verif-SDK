setlocal
set PATH=%PATH%;../../../binaries/windows/x86_64
java -Djava.library.path=../../../binaries/windows/x86_64 Verify --image "../../../assets/images/United States - California Driving License (2017).jpg" --assets ../../../assets
endlocal