import os
import io
import sys
import time
import subprocess
from timeit import timeit

def format_tests(file_name):
    # Remove % character from benchmarks
    with open(file_name, "r") as f:
        string = ""
        for line in f:
            if line == "%\n":
                pass
            elif line == "0\n":
                pass
            else:
              string += line
    with open(file_name, "w") as f:
        f.write(string)

#saved_stderr = sys.stdout
#saved_stdout = sys.stdout
#new_stdout = io.StringIO()
#sys.stdout = new_stdout
#sys.stderr = new_stdout

#testing = generated_sat_path

def run_test(tests, loop=10):
    #file_null = open(os.devnull, 'w')
    results = ""
    count = 1
    for root, dirs, files in os.walk(tests):
        for name in files:
            if name.endswith((".cnf")):
                file_name = os.path.abspath(os.path.join(root,name))
                format_tests(file_name)
               # sp = subprocess.call(["minisat/core/minisat_static", "{file}".format(file=file_name)])
    #            sp.communicate()
                avg = 0
                #loop = 10
                unsat = 0
                sat = 0

                for i in range(0,loop):
                    setup = '''
import subprocess

file = open("minisat_results/{file_name}.tmp.{num}", "w")
sys.stdout = file
'''.format(file_name=name,num=i)
       
                    #time_taken=timeit(stmt = "subprocess.Popen([\"minisat/core/minisat_static\", \"{file_name}\"], stdout=file)".format(
                    #    file_name=file_name), setup=setup, number=1)
 #                   sys.stderr.write("PING")
                    file = open("minisat_results/{file_name}.tmp.{num}".format(file_name=name, num=i), "w")
                    t0 = time.time()
                    sp = subprocess.Popen(["minisat/core/minisat_static", "{file_name}".format(file_name=file_name)], stdout=file)
                    sp.communicate()
                    t1 = time.time()
                    avg += (t1-t0)
#                    print(str(t1-t0) + name)
                    file.close()
#                    sys.stderr.write("PING2")
#                    avg += time_taken
 #                   r = name + " " + str(avg/loop) + "\n"
  #                  sys.stderr.write(r)
                #s_time = os.times()
                #print(s_time)
                #print(str(s_time[2]) + " " + str(s_time[3]))
                for i in range(0, loop):
                    path = "minisat_results/{file_name}.tmp.{num}".format(file_name=name, num=i)
                    with open(path, "r") as f:
                        seen_results = False
                        for line in f:
                            if line.strip() == "UNSATISFIABLE":
                                unsat += 1
                                seen_results = True
                                break
                            elif line.strip() == "SATISFIABLE":
                                sat += 1
                                seen_results = True
                                break
                        if seen_results:
                            os.remove(path)
                        #       sys.stderr.write(line)
                r = name + " " + str(avg/loop) + " sat:" + str(sat) + " unsat:" + str(unsat) + "\n"
                #sys.stderr.write(r)
                # print(r)
                # print("minisat finished test {num}".format(num=count))
                results += r
                count += 1
    with open("minisat_results/{tests}".format(tests=tests), "w") as o:
        o.write(results)

#Code starts here
#benchmarks = [["uf20-91", 100]]
benchmarks = [["uf20-91", 50], ["uf100-430", 50], ["UUF100.430.1000", 50], ["UF125.538.100", 50],["UUF125.538.100", 50]]
#benchmarks = ["uf20-91", "uf100-430", "UUF100.430.1000", "UF125.538.100","UUF125.538.100", "UF250.1065.100", "UUF250.1065.100"]
for bm in benchmarks:
    run_test(bm[0], bm[1])