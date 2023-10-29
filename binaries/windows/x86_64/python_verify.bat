setlocal
set PYTHONPATH=%PYTHONPATH%;.;../../../python
set PATH=%PATH%;%~dp0
python ../../../samples/python/verify/verify.py --image "../../../assets/images/United States - California Driving License (2017).jpg" --assets ../../../assets
endlocal