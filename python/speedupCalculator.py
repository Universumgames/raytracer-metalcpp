import itertools
import pandas as pd
import plotly.express as px

from rayTracerRunner import RayTracerImplementation

time_log_path = "../timelog.csv"
graph_path = "../timelog_plot.html"


def read_timelog(file_path) -> pd.DataFrame:
    df = pd.read_csv(file_path)
    df["Workload"] = df["Rays"] * df["Bounces"] * df["Triangles"]
    return df


def filter_timelog_by_implementation(df: pd.DataFrame, implementation: RayTracerImplementation) -> pd.DataFrame:
    return df[df['implementation'] == implementation.to_c_class_name()]


if __name__ == "__main__":
    df = read_timelog(time_log_path)

    # Prepare plot
    # plt.figure(figsize=(12, 8))
    #
    # line_styles = itertools.cycle(["-", "--", "-.", ":"])
    # markers = itertools.cycle(["o", "s", "D", "^", "v", "x"])
    #
    # # Group by Implementation and Git Hash
    # for (impl, git_hash), group_impl in df.groupby(["Implementation", "Git Hash"]):
    #     style = next(line_styles)
    #     marker = next(markers)
    #     label = f"{impl} | {git_hash[:7]}"
    #
    #     # Sort by workload for smoother lines
    #     group_impl = group_impl.sort_values("Workload")
    #
    #     plt.plot(
    #         group_impl["Workload"],
    #         group_impl["Total Duration (ms)"],
    #         linestyle=style,
    #         marker=marker,
    #         label=label,
    #     )
    #
    # plt.xlabel("Workload (Rays × Bounces × Triangles)")
    # plt.ylabel("Total Duration (ms)")
    # plt.title("Total Duration vs Workload by Implementation and Git Hash")
    # plt.legend(bbox_to_anchor=(1.05, 1), loc="upper left", fontsize="small")
    # plt.xscale("log")
    # plt.yscale("log")
    # plt.grid(True)
    # plt.tight_layout()
    # plt.show()

    # Create interactive scatter/line plot
    fig = px.line(
        df.sort_values("Workload"),
        x="Workload",
        y="Total Duration (ms)",
        color="Implementation",
        # line_group="Git Hash",
        line_dash="Rays",
        facet_col="Filename",
        markers=True,
        hover_data={
            "Implementation": True,
            "Git Hash": True,
            "Rays": True,
            "Samples": True,
            "Bounces": True,
            "Triangles": True,
            "Spheres": True,
            "Encoding(ms)": True,
            "Raytracing(ms)": True,
            # "Decoding(ms)": True,
            "Total Duration (ms)": True,
            "Workload": True,
            "Filename": True,
        },
        title="Total Duration vs Workload by Implementation and Git Hash",
        log_x=True,
        # log_y=True,
    )

    fig.update_layout(
        xaxis_title="Workload (Rays × Bounces × Triangles)",
        yaxis_title="Total Duration (ms)",
        hovermode="closest",
    )
    fig.write_html(graph_path)
    fig.show()
