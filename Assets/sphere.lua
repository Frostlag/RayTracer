-- A simple scene with five spheres

mat1 = gr.material({0.7, 1.0, 0.7}, {0.1, 0.1, 0.1}, 25)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25)
tex1 = gr.texture("colour-grid.png")
scene_root = gr.node('root')

s1 = gr.mesh("sphere","Assets/sphere.obj")
s1:set_material(mat1)
scene_root:add_child(s1)


white_light = gr.light({100.0, 10.0, 100.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
--orange_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(scene_root, 'sphere.png', 512, 512,
	  {0, 0, 5}, {0, 0, -800}, {0, 1, 0}, 50,
	  {0.1, 0.1, 0.1}, {white_light})
