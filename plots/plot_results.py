import csv
import statistics
import matplotlib.pyplot as plt


def read_csv(path):
    with open(path, newline="") as file:
        return list(csv.DictReader(file))


def plot_random_results():
    rows = read_csv("benchmarks/random_results.csv")

    grouped = {}

    for row in rows:
        n = int(row["text_length"])

        if n not in grouped:
            grouped[n] = {
                "naive": [],
                "kmp": []
            }

        grouped[n]["naive"].append(float(row["naive_us"]))
        grouped[n]["kmp"].append(float(row["kmp_us"]))

    lengths = sorted(grouped.keys())

    naive = [
        statistics.median(grouped[n]["naive"])
        for n in lengths
    ]

    kmp = [
        statistics.median(grouped[n]["kmp"])
        for n in lengths
    ]

    plt.figure()

    plt.plot(lengths, naive, marker="o", label="Naive")
    plt.plot(lengths, kmp, marker="o", label="KMP")

    plt.xlabel("Text length")
    plt.ylabel("Median runtime (microseconds)")
    plt.title("Random Input Runtime")
    plt.legend()
    plt.grid(True)

    plt.tight_layout()
    plt.savefig("plots/random_runtime.png", dpi=300)
    plt.close()


def plot_adversarial_results():
    rows = read_csv("benchmarks/adversarial_results.csv")

    lengths = [int(row["text_length"]) for row in rows]
    naive = [float(row["naive_us"]) for row in rows]
    kmp = [float(row["kmp_us"]) for row in rows]

    plt.figure()

    plt.plot(lengths, naive, marker="o", label="Naive")
    plt.plot(lengths, kmp, marker="o", label="KMP")

    plt.xlabel("Text length")
    plt.ylabel("Median runtime (microseconds)")
    plt.title("Adversarial Input Runtime")
    plt.legend()
    plt.grid(True)

    plt.tight_layout()
    plt.savefig("plots/adversarial_runtime.png", dpi=300)
    plt.close()


def plot_pattern_length_results():
    rows = read_csv("benchmarks/pattern_length_results.csv")

    lengths = [int(row["pattern_length"]) for row in rows]
    naive = [float(row["naive_us"]) for row in rows]
    kmp = [float(row["kmp_us"]) for row in rows]

    plt.figure()

    plt.plot(lengths, naive, marker="o", label="Naive")
    plt.plot(lengths, kmp, marker="o", label="KMP")

    plt.xlabel("Pattern length")
    plt.ylabel("Median runtime (microseconds)")
    plt.title("Runtime as Pattern Length Increases")
    plt.legend()
    plt.grid(True)

    plt.tight_layout()
    plt.savefig("plots/pattern_length_runtime.png", dpi=300)
    plt.close()


def plot_comparisons():
    rows = read_csv("benchmarks/pattern_length_results.csv")

    lengths = [int(row["pattern_length"]) for row in rows]

    naive = [
        int(row["naive_comparisons"])
        for row in rows
    ]

    kmp = [
        int(row["kmp_comparisons"])
        for row in rows
    ]

    plt.figure()

    plt.plot(lengths, naive, marker="o", label="Naive")
    plt.plot(lengths, kmp, marker="o", label="KMP")

    plt.xlabel("Pattern length")
    plt.ylabel("Character comparisons (log scale)")
    plt.title("Character Comparisons as Pattern Length Increases")
    plt.legend()
    plt.grid(True)

    plt.yscale("log")
    plt.tight_layout()
    plt.savefig("plots/pattern_length_comparisons.png", dpi=300)
    plt.close()


if __name__ == "__main__":
    plot_random_results()
    plot_adversarial_results()
    plot_pattern_length_results()
    plot_comparisons()

    print("Plots created successfully.")