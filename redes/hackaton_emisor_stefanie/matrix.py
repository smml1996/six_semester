import scipy.misc
import numpy as np
import pandas as pd

temp = scipy.misc.imread("poni.png", mode="L");
df = pd.DataFrame(temp)
df.to_csv('img.csv', sep=',', encoding='utf-8', index=False, header=False)
