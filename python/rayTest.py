import numpy as np
import plotly.graph_objects as go

import pixels
import rays

# Example: vectors from the origin
vectors = rays.vectors
pixels = pixels.pixels

if __name__ == "__main__":
    # Create quiver (arrows)
    fig = go.Figure()

    for i in range(0,len(vectors)):
        # Arrow line
        fig.add_trace(go.Scatter3d(
            x=[pixels[i][0], pixels[i][0] + vectors[i][0]],
            y=[pixels[i][1], pixels[i][1] + vectors[i][1]],
            z=[pixels[i][2], pixels[i][2] + vectors[i][2]],
            mode='lines+markers',
            line=dict(width=6, color='blue'),
            marker=dict(size=3, color='red')
        ))

    # Axis setup
    fig.update_layout(
        scene=dict(
            xaxis_title='X',
            yaxis_title='Y',
            zaxis_title='Z',
            aspectmode='cube',
        ),
        title='3D Vector Visualization'
    )

    fig.show()