from openpyxl import load_workbook
import matplotlib.pyplot as plt
import matplotlib.ticker

if __name__ == "__main__":
    workbook = load_workbook(filename="WorkGroups.xlsx", data_only=True)
    print(f"Generating Graphs")
    for sheet in workbook.worksheets:
        # print(sheet.title)
        name = sheet.cell(1, 1).value
        print(f'Generating graph for "{name}"')
        x_title = sheet.cell(2, 1).value
        y_title = sheet.cell(2, 3).value
        k = 3
        x_data = []
        y_data = []
        while True:
            if sheet.cell(k, 1).value:
                x_data.append(sheet.cell(k, 1).value)
                y_data.append(sheet.cell(k, 3).value)
                k += 1
                continue
            break
        fig = plt.figure()
        ax = fig.add_subplot(111)
        fig.get_axes()[0].set_xlabel(x_title)
        fig.get_axes()[0].set_ylabel(y_title)
        ax.set_xscale("log", base=2)
        ax.get_xaxis()
        ax.plot(x_data, y_data, "o")
        ax.get_xaxis().set_major_formatter(matplotlib.ticker.ScalarFormatter())
        ax.get_xaxis().set_minor_formatter(matplotlib.ticker.NullFormatter())
        # plt.xlabel(x_title)
        # plt.ylabel(y_title)
        plt.title(name)
        # plt.xscale("log", base=2)
        plt.savefig(f"{name.replace(' ', '_')}.pdf")
        plt.close()
