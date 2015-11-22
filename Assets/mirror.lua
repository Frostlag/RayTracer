
mat1 = gr.new_material({0.5, 0.5, 0.5}, {0,0,0}, 25,1)
newmat2 = gr.new_material({0.5, 0.5, 0.5}, {0, 0, 0}, 25, 1)

scene_root = gr.node('root')

s1 = gr.cube('mirror')
s1:translate(-0.5,-0.5,0)
s1:scale(200,200,200)
s1:translate(0,0,-400)
scene_root:add_child(s1)
s1:set_material(newmat2)

s2 = gr.cube('cube')
s2:translate(-0.5,-0.5,0)
s2:scale(400,400,400)
s2:translate(0,0,250)
scene_root:add_child(s2)
s2:set_material(mat1)



white_light = gr.light({0,0,200}, {0.9, 0.9, 0.9}, {1, 0, 0})
--orange_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(scene_root, 'mirror.png', 512*2, 512*2,
	  {0, 0, 200}, {0, 0, -200}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {white_light, orange_light})
