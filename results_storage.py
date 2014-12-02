store = {}

results_dir = ["minisat_results", "minisat_2005_results", "zchaff_results", "picosat_960_results", "walksat_51_results"]
benchmarks = ["uf20-91", "uf100-430", "UUF100.430.1000", "UF125.538.100","UUF125.538.100"]
#benchmarks = ["uf20-91", "uf100-430", "UUF100.430.1000", "UF125.538.100","UUF125.538.100", "UF250.1065.100", "UUF250.1065.100"]

for result in results_dir:
	store[result] = {}
	try:
		for bm in benchmarks:
			total = 0
			with open("{dir}/{test}".format(dir=result, test=bm), "r") as f:
				# print("{dir}/{test}".format(dir=result, test=bm))
				store[result][bm] = {}
				for line in f:
					split = line.split()
					store[result][bm][split[0]] = split[1:]
					total += float(split[1])
			store[result][bm]["__total"] = total
	except:
		continue

def get_totals():
	for result in results_dir:
		for bm in benchmarks:
			print(result + " " + bm + " " + str(store[result][bm]["__total"]))

def get_results_cnf(bm, cnf):
	for result in results_dir:
			print(result)
			print(store[result][bm][cnf])

#print(store["zchaff_results"]["uf20-91"]["__total"])
#get_results_cnf("uf20-91", "uf20-0321.cnf")

get_totals()