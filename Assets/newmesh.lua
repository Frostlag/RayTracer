-- A simple scene with five spheres

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25)
tex1 = gr.texture("Assets/BigSister.png")
scene_root = gr.node('root')

s1 = gr.mesh("test","Assets/Eleanor.obj")
s1:rotate("y",10)
s1:scale(40,40,40)
s1:translate(-10,-90,0)
--s1:scale(1,10,10)
scene_root:add_child(s1)
s1:set_material(mat1)
s1:set_texture(tex1);



white_light = gr.light({-100.0, 0.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})

gr.render(scene_root, 'new_mesh.png', 128*8, 128*8,
	  {0, 0, 300}, {0, 0, -800}, {0, 1, 0}, 50,
	  {0.5, 0.5, 0.5}, {white_light})
