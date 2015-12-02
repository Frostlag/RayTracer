
mat1 = gr.material({0, 0.5, 0.5}, {0.5, 0.7, 0.5}, 50)
scene_root = gr.node('root')
torus = gr.torus('torus',3,10)
torus:rotate("x",30)
--torus:scale(0.1,0.1,0.1)
torus:set_material(mat1)
scene_root:add_child(torus)



white_light = gr.light({100.0, 0.0, 300.0}, {0.9, 0.9, 0.9}, {1, 0, 0})

gr.render(scene_root, 'torus.png', 128*4, 128*4,
	  {0, 0, 30}, {0, 0, -800}, {0, 1, 0}, 50,
	  {0.5, 0.5, 0.5}, {white_light})
