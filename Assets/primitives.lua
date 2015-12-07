-- A simple scene with five spheres

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25)
tex1 = gr.texture("colour-grid.png")
scene_root = gr.node('root')

s1 = gr.cone('cone')
s1:rotate("x",30)
s1:scale(5,5,5)
s1:translate(-15,0,0)
scene_root:add_child(s1)
s1:set_material(mat1)

s2 = gr.cylinder('cylinder')
s2:rotate("x",30)
s2:scale(5,5,5)
s2:translate(15,0,0)
scene_root:add_child(s2)
s2:set_material(mat1)

s3 = gr.torus('torus',2,5)
s3:rotate("x",30)
s3:set_material(mat2)
scene_root:add_child(s3)



white_light = gr.light({100.0, 10.0, 100.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
--orange_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(scene_root, 'primitives.png', 1024, 1024,
	  {0, 0, 50}, {0, 0, -800}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {white_light})
