import pandas as pd
import plotly.express as px

from rayTracerRunner import RayTracerImplementation

time_log_path = "../timelog.csv"
graph_path = "../speedup.html"

reference_impl = RayTracerImplementation.SEQUENTIAL.to_c_class_name()[0]

group_cols = [
    "Platform", "Architecture", "Filename", "Samples", "Bounces", "Rays",
    "Width", "Height", "Triangles", "Spheres"
]

time_col = "Total Duration (ms)"

def read_timelog(file_path) -> pd.DataFrame:
    df = pd.read_csv(file_path)
    df["Workload"] = df["Rays"] * df["Bounces"] * df["Triangles"]
    return df


def filter_timelog_by_implementation(df: pd.DataFrame, implementation: RayTracerImplementation) -> pd.DataFrame:
    return df[df['implementation'] == implementation.to_c_class_name()]


if __name__ == "__main__":
    df = read_timelog(time_log_path)

    df["Workload"] = df["Rays"] * df["Bounces"] * df["Triangles"]

    # Choose the speedup column
    speedup_col = f"Speedup vs {reference_impl}"

    ref_times = (
        df[df["Implementation"] == reference_impl]
        .groupby(group_cols)[time_col]
        .mean()
        .rename("Reference Time")
    )

    # Merge reference times back into the main DataFrame
    df = df.merge(ref_times, on=group_cols, how="left")

    # Compute speedup: reference time / current time
    df[speedup_col] = df["Reference Time"] / df[time_col]

    # filter out rows without reference
    df = df.dropna(subset=["Speedup vs " + reference_impl])

    # Create the scatter plot
    fig = px.line(
        df.sort_values("Workload"),
        x="Workload",
        y=speedup_col,
        color="Implementation",  # color by implementation
        line_group="Bounces",
        line_dash="Samples",
        symbol="Triangles",
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
            "Decoding(ms)": True,
            "Total Duration (ms)": True,
            "Workload": True,
            "Filename": True,
            "Width": True,
            "Height": True,
        },
        title=f"Speedup vs {reference_impl} by Workload",
        log_x=True,
        log_y=True,
    )

    fig.write_html(graph_path)
    fig.show()
