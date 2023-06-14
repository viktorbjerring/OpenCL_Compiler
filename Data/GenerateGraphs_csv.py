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
    names = ("Intel Core i7-8750H (2.2GHz)", "Intel Core i7-11800H (2.30GHz)", "Intel UHD Graphics 630", "Intel Core i7-11800H (2.30GHz) (Map)", "NVIDIA GTX 1050 (Laptop)", "NVIDIA RTX 3060 (Laptop)")
    names_a = names[::2]
    names_b = names[1::2]
    opencl_intel_data, opencl_title, cpp_intel_data, cpp_titles, create_intel_data, create_title = read_csv("Intel(R) Core(TM) i7-8750H CPU @ 2.20GHz.csv", True)
    opencl_intel_map_data, cpp_intel_map_data, create_intel_map_data = read_csv("Intel(R) OpenCL.csv")
    opencl_intelG_data, cpp_intelG_data, create_intelG_data = read_csv("Intel(R) UHD Graphics 630.csv")
    opencl_intelG_map_data, cpp_intelG_map_data, create_intelG_map_data = read_csv("Intel(R) OpenCL Map.csv")
    opencl_nvidia_data, cpp_nvidia_data, create_nvidia_data = read_csv("NVIDIA GeForce GTX 1050.csv")
    opencl_nvidia_map_data, cpp_nvidia_map_data, create_nvidia_map_data = read_csv("NVIDIA CUDA.csv")
    opencl_data_a = {}
    opencl_data_b = {}
    cpp_data_a = {}
    cpp_data_b = {}
    create_data_a = {}
    create_data_b = {}

    print(opencl_intel_data)
    # print(opencl_intel_map_data)
    
    for i, item in enumerate(opencl_title):
        item = item.capitalize()
        opencl_data_a[item] = np.array([opencl_intel_data[i], opencl_intelG_data[i], opencl_nvidia_data[i]])
        opencl_data_b[item] = np.array([opencl_intel_map_data[i], opencl_intelG_map_data[i], opencl_nvidia_map_data[i]])

    for i, item in enumerate(cpp_titles):
        item = item.capitalize()
        cpp_data_a[item] = np.array([cpp_intel_data[i], cpp_intelG_data[i], cpp_nvidia_data[i]])
        cpp_data_b[item] = np.array([cpp_intel_map_data[i], cpp_intelG_map_data[i], cpp_nvidia_map_data[i]])

    create_data_a = cpp_data_a.copy()
    create_data_b = cpp_data_b.copy()
    # create_data[create_title] = np.array([create_intel_data, create_intel_map_data, create_intelG_data, create_intelG_map_data, create_nvidia_data, create_nvidia_map_data])
    create_data_a[create_title.capitalize()] = np.array([create_intel_data, create_intelG_data, create_nvidia_data])
    create_data_b[create_title.capitalize()] = np.array([create_intel_map_data, create_intelG_map_data, create_nvidia_map_data])

    
    fig, (ax1, ax2) = plt.subplots(1, 2)
    bottom = np.zeros(3)

    width = 0.5

    for label, data_count in create_data_b.items():
        print(f"{label}: {data_count}")
        p = ax1.bar(names_b, data_count, width, label=label, bottom=bottom)
        bottom += data_count

    bottom = np.zeros(3)
    for label, data_count in create_data_a.items():
        print(f"{label}: {data_count}")
        p = ax2.bar(names_a, data_count, width, label=label, bottom=bottom)
        bottom += data_count

    ax2.legend(loc="center left", bbox_to_anchor=(1.05, 0.5))
    ax1.set_ylabel("Execution time [ms]")

    ax1.tick_params(axis='x', labelrotation=270)
    ax2.tick_params(axis='x', labelrotation=270)
    fig.align_xlabels()

    plt.title("Time to execute, separated into segments")
    # plt.xticks(rotation="vertical")

    plt.savefig("exe_shit_time.pdf", bbox_inches="tight")

    fig, (ax1, ax2) = plt.subplots(1, 2)
    bottom = np.zeros(3)

    width = 0.5

    for label, data_count in cpp_data_b.items():
        print(f"{label}: {data_count}")
        p = ax1.bar(names_b, data_count, width, label=label, bottom=bottom)
        bottom += data_count

    bottom = np.zeros(3)
    for label, data_count in cpp_data_a.items():
        print(f"{label}: {data_count}")
        p = ax2.bar(names_a, data_count, width, label=label, bottom=bottom)
        bottom += data_count

    ax2.legend(loc="center left", bbox_to_anchor=(1.05, 0.5))
    ax1.set_ylabel("Execution time [ms]")

    ax1.tick_params(axis='x', labelrotation=270)
    ax2.tick_params(axis='x', labelrotation=270)
    fig.align_xlabels()

    plt.title("Time to execute, separated into segments (Excluding create kernel)")
    # plt.xticks(rotation="vertical")

    plt.savefig("exe_shit_time_no_kernel.pdf", bbox_inches="tight")

    fig, (ax1, ax2) = plt.subplots(1, 2)
    bottom = np.zeros(3)

    width = 0.5

    for label, data_count in opencl_data_b.items():
        print(f"{label}: {data_count}")
        p = ax1.bar(names_b, data_count, width, label=label, bottom=bottom)
        bottom += data_count

    bottom = np.zeros(3)
    for label, data_count in opencl_data_a.items():
        print(f"{label}: {data_count}")
        p = ax2.bar(names_a, data_count, width, label=label, bottom=bottom)
        bottom += data_count

    ax2.legend(loc="center left", bbox_to_anchor=(1.05, 0.5))
    ax1.set_ylabel("Execution time [us]")

    ax1.tick_params(axis='x', labelrotation=270)
    ax2.tick_params(axis='x', labelrotation=270)
    fig.align_xlabels()

    plt.title("Time to execute kernel, separated into segments")
    # plt.xticks(rotation="vertical")

    plt.savefig("exe_shit_time_kernel.pdf", bbox_inches="tight")
