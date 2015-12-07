-- A simple scene with five spheres

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25)
tex1 = gr.texture("colour-grid.png")
scene_root = gr.node('root')

s1 = gr.cube('cube')
s1:translate(-0.5,-0.5,-0.5)
s1:rotate("y",90)
s1:scale(2,2,2)
s1:translate(-0.4,0,0)
s1:set_material(mat1)
s1:set_texture(tex1)

s2 = gr.sphere('sphere')
s2:scale(2,2,2)
s2:translate(0.5,0,0)
s2:set_material(mat1)
s2:set_texture(tex1)

s3 = gr.union("union",s1,s2)
s3:translate(-4,0,0)
s3:set_material(mat2)
scene_root:add_child(s3)

s4 = gr.conjunction("conjunction",s1,s2)
s4:translate(0,0,0)
s4:set_material(mat2)
scene_root:add_child(s4)

s5 = gr.subtraction("subtraction",s1,s2)
s5:translate(4,0,0)
s5:set_material(mat2)
scene_root:add_child(s5)

white_light = gr.light({100.0, 10.0, 100.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
--orange_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(scene_root, 'csg.png', 1024*0.5, 1024*0.5,
	  {0, 0, 15}, {0, 0, -800}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {white_light})
