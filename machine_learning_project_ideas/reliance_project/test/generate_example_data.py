import numpy as np
import pandas as pd
import pubchempy as pcp

total_data=10

df=pd.DataFrame()
  	
df['Exp. Id']=[f'Exp.Id-{i}'for i in np.random.randint(1,total_data,size=total_data)]

df['Reactant_1']=[pcp.Compound.from_cid(int(cid)).iupac_name for cid in np.random.randint(1,total_data,size=total_data)]

df['Reactant_2']=[pcp.Compound.from_cid(int(cid)).iupac_name for cid in np.random.randint(total_data,2*total_data,size=total_data)]

df['']=[pcp.Compound.from_cid(int(cid)).iupac_name for cid in np.random.randint(total_data,2*total_data,size=total_data)]

print(df)
df.to_csv('example_dataset.csv',index=False)
