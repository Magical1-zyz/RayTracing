import subprocess
import numpy as np
import plotly.graph_objects as go
import os


def run_renderer(exe_path, output_file):
    """
    运行外部渲染器，并将输出写入 output_file 文件。
    """
    # 调用外部程序，并将其标准输出写入文件
    with open(output_file, 'w') as f:
        subprocess.run([exe_path], stdout=f, check=True)


def plot_data(data_file):
    """
    从 data_file 中读取 x, y, z 数据，并绘制 3D 散点图。
    """
    # 读取数据，假设每行格式为 "x y z"
    data = np.loadtxt(data_file)
    x = data[:, 0]
    y = data[:, 1]
    z = data[:, 2]

    fig = go.Figure(data=[go.Scatter3d(
        x=x, y=y, z=z,
        mode='markers',
        marker=dict(
            size=4,
            color=z,  # 根据 z 值上色，可按需调整
            colorscale='Viridis',
            opacity=0.8
        )
    )])

    fig.update_layout(
        scene=dict(
            xaxis_title='X',
            yaxis_title='Y',
            zaxis_title='Z'
        ),
        title='3D Scatter Plot of Rendered Data'
    )

    fig.show()


if __name__ == '__main__':
    # 定义渲染器可执行文件路径和数据输出文件路径
    exe_path = os.path.join("../bin", "sphere_plot.exe")
    output_file = "data.txt"

    # 运行渲染器，并生成 data.txt
    run_renderer(exe_path, output_file)

    # 使用 Plotly 绘制 data.txt 中的数据
    plot_data(output_file)
