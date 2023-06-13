import matplotlib.pyplot as plt
import matplotlib.ticker
import csv
import numpy as np

def read_csv(file, ret_titles=False):
    with open(file, 'r') as csvfile:
        csv_obj = csv.reader(csvfile)
        data_arr = []
        for row in csv_obj:
            data_arr.append(row)
        titles = [tit.lstrip() for tit in data_arr[0]]
        np_arr = np.array(data_arr[1:])
        np_arr = np_arr.astype(np.int32)
        np_arr = np.transpose(np_arr)
        np_arr = np.average(np_arr, axis=1)
        for i, title in enumerate(titles):
            if "execution" in title:
                np_arr = np.delete(np_arr, i, 0)
                del titles[i] 
        opencl_arr = []
        opencl_tit = []
        stdcpp_arr = []
        stdcpp_tit = []
        create_time = 0
        create_tit = ""
        for i, title in enumerate(titles):
            if title[-4:] == "[ns]":
                opencl_arr.append(np_arr[i] / 1000)
                opencl_tit.append(title.replace("[ns]", "[us]"))
            elif "create" in title:
                create_tit = title.replace("[us]", "[ms]")
                create_time = np_arr[i] / 1000
            else:
                stdcpp_tit.append(title.replace("[us]", "[ms]"))
                stdcpp_arr.append(np_arr[i] / 1000)
        if ret_titles:
            return (opencl_arr, opencl_tit, stdcpp_arr, stdcpp_tit, create_time, create_tit)
        else:
            return (opencl_arr, stdcpp_arr, create_time)


if __name__ == "__main__":
    names = ("Intel Core i7-8750H (2.2GHz)", "Intel Core i7-8750H (2.2GHz) (Map)", "Intel UHD Graphics 630", "Intel UHD Graphics 630 (Map)", "NVIDIA GTX 1050 (Laptop)", "NVIDIA GTX 1050 (Laptop) (Map)")
    names = names[::2]
    opencl_intel_data, opencl_title, cpp_intel_data, cpp_titles, create_intel_data, create_title = read_csv("Intel(R) Core(TM) i7-8750H CPU @ 2.20GHz.csv", True)
    # opencl_intel_map_data, cpp_intel_map_data, create_intel_map_data = read_csv("Intel(R) Core(TM) i7-8750H CPU @ 2.20GHzMap.csv")
    opencl_intelG_data, cpp_intelG_data, create_intelG_data = read_csv("Intel(R) UHD Graphics 630.csv")
    # opencl_intelG_map_data, cpp_intelG_map_data, create_intelG_map_data = read_csv("Intel(R) UHD Graphics 630Map.csv")
    opencl_nvidia_data, cpp_nvidia_data, create_nvidia_data = read_csv("NVIDIA GeForce GTX 1050.csv")
    # opencl_nvidia_map_data, cpp_nvidia_map_data, create_nvidia_map_data = read_csv("NVIDIA GeForce GTX 1050Map.csv")
    opencl_data = {}
    cpp_data = {}
    create_data = {}

    print(opencl_intel_data)
    # print(opencl_intel_map_data)
    
    for i, item in enumerate(opencl_title):
        # opencl_data[item] = np.array([opencl_intel_data[i], opencl_intel_map_data[i], opencl_intelG_data[i], opencl_intelG_map_data[i], opencl_nvidia_data[i], opencl_nvidia_map_data[i]])
        opencl_data[item] = np.array([opencl_intel_data[i], opencl_intelG_data[i], opencl_nvidia_data[i]])

    for i, item in enumerate(cpp_titles):
        # cpp_data[item] = np.array([cpp_intel_data[i], cpp_intel_map_data[i], cpp_intelG_data[i], cpp_intelG_map_data[i], cpp_nvidia_data[i], cpp_nvidia_map_data[i]])
        cpp_data[item] = np.array([cpp_intel_data[i], cpp_intelG_data[i], cpp_nvidia_data[i]])

    create_data = cpp_data.copy()
    # create_data[create_title] = np.array([create_intel_data, create_intel_map_data, create_intelG_data, create_intelG_map_data, create_nvidia_data, create_nvidia_map_data])
    create_data[create_title] = np.array([create_intel_data, create_intelG_data, create_nvidia_data])

    
    fig, ax = plt.subplots()
    bottom = np.zeros(3)

    width = 0.5

    for label, data_count in create_data.items():
        print(f"{label}: {data_count}")
        p = ax.bar(names, data_count, width, label=label, bottom=bottom)
        bottom += data_count

    ax.set_title("Time to execute, separated into segments")
    ax.legend(loc="center left", bbox_to_anchor=(1.05, 0.5))

    ax.set_ylabel("Execution time [ms]")

    plt.xticks(rotation="vertical")

    plt.savefig("exe_shit_time.pdf", bbox_inches="tight")


    fig, ax = plt.subplots()
    bottom = np.zeros(3)

    width = 0.5

    for label, data_count in cpp_data.items():
        print(f"{label}: {data_count}")
        p = ax.bar(names, data_count, width, label=label, bottom=bottom)
        bottom += data_count

    ax.set_title("Time to execute separated into segments (Excluding create kernel)")
    ax.legend(loc="center left", bbox_to_anchor=(1.05, 0.5))

    ax.set_ylabel("Execution time [ms]")

    plt.xticks(rotation="vertical")

    plt.savefig("exe_shit_time_no_create.pdf", bbox_inches="tight")

    fig, ax = plt.subplots()
    bottom = np.zeros(3)

    width = 0.5

    for label, data_count in opencl_data.items():
        print(f"{label}: {data_count}")
        p = ax.bar(names, data_count, width, label=label, bottom=bottom)
        bottom += data_count

    ax.set_title("Time to execute kernel, seperated into segments")
    ax.legend(loc="center left", bbox_to_anchor=(1.05, 0.5))

    ax.set_ylabel("Execution time [us]")

    plt.xticks(rotation="vertical")

    plt.savefig("exe_shit_time_opencl.pdf", bbox_inches="tight")