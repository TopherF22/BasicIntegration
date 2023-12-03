import yfinance as yf
import pandas as pd
import datetime as dt
import matplotlib.pyplot as plt
import os
import sys
import subprocess

# check arguments
if len(sys.argv) < 2:
    print("Usage: python integrator.py <ticker1> [<ticker2> ...]")
    sys.exit(1)
tickers = sys.argv[1:]
    
 
# set time   
now = dt.datetime.now()
yearAgo = now - dt.timedelta(days=365)


#download data
try:
    data = yf.download(tickers, start=yearAgo, end=now)['Adj Close']
except Exception as e:
    print(f"Error retrieving data: {e}")
    sys.exit(1)
print("Saving data to:", 'data.csv')
pd.DataFrame(data).to_csv('data.csv')



# call c++ program py++ to calculate mean
try:
    result = subprocess.run(['./py++', 'data.csv'], check=True)
except subprocess.CalledProcessError as e:
    print(f"Execution failed: {e}")
    
results = pd.read_csv('output.csv')
print(results)
# # plot
# plt.figure(figsize=(10, 6))
# plt.plot(data_df['Date'], data_df['Adj Close'], label='Adj Close')
# plt.axhline(y=results[0], color='r', linestyle='--', label='Average')
# plt.title(f'{sys.argv[1]} Price Over Time')
# plt.xlabel('Date')
# plt.ylabel('Adj Close')
# plt.legend()
# plt.grid(True)
# plt.show()

# # delete csv
# os.remove('data.csv')
# os.remove('output.csv')

