-- A simple scene with five spheres

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25)
scene_root = gr.node('root')

table = gr.node('table')
table_top = gr.cube("table_top")
table_top:translate(-0.5,-0.5,-0.5)
table_top:scale(100,5,50)
table_top:set_material(mat1)
table:add_child(table_top)

table_leg = gr.cylinder("table_leg")
table_leg:translate(0,-1,0)
table_leg:scale(2,20,2)
table_leg:set_material(mat1)

table_leg_x_constant = 40
table_leg_y_constant = 20
table_leg1 = gr.node("table_leg1")
table_leg1:add_child(table_leg)
table_leg1:translate(table_leg_x_constant,0,table_leg_y_constant)
table:add_child(table_leg1)

table_leg2 = gr.node("table_leg1")
table_leg2:add_child(table_leg)
table_leg2:translate(-table_leg_x_constant,0,table_leg_y_constant)
table:add_child(table_leg2)
table_leg3 = gr.node("table_leg3")
table_leg3:add_child(table_leg)
table_leg3:translate(table_leg_x_constant,0,-table_leg_y_constant)
table:add_child(table_leg3)
table_leg4 = gr.node("table_leg1")
table_leg4:add_child(table_leg)
table_leg4:translate(-table_leg_x_constant,0,-table_leg_y_constant)
table:add_child(table_leg4)
--table:rotate("x",-90)

--scene_root:add_child(table)
uupa_base = gr.sphere("uupa_base")
uupa_base:scale(5,5,5)
uupa_ring = gr.torus("uupa_ring",4,100)
uupa_ring:scale(0.05,0.05,0.05)
uupa_body = gr.subtraction("uupa_body",uupa_base,uupa_ring)
uupa_body:set_material(mat1)

uupa_eye_base = gr.sphere("uupa_eye_base")
uupa_eye_base:scale(0.5,0.5,0.5)
uupa_eye_dimple = gr.sphere("uupa_eye_dimple")
uupa_eye_dimple:scale(0.4,0.4,0.4)
uupa_eye_dimple:translate(0,0,0.2)
uupa_eye = gr.subtraction("uupa_eye",uupa_eye_base,uupa_eye_dimple)
uupa_eye:set_material(mat1)
uupa_eye:translate(0,1.5,4)
uupa_eye1 = gr.node("uupa_eye1")
uupa_eye1:add_child(uupa_eye)
uupa_eye1:translate(-2,0,0)

uupa_eye2 = gr.node("uupa_eye2")
uupa_eye2:add_child(uupa_eye)
uupa_eye2:translate(2,0,0)

uupa = gr.node("uupa")
uupa:add_child(uupa_body)
uupa:add_child(uupa_eye1)
uupa:add_child(uupa_eye2)
uupa:scale(10,10,10)
--uupa:rotate("x",40)
uupa:rotate("y",90)
scene_root:add_child(uupa)

white_light = gr.light({100.0, 0.0, 300.0}, {0.9, 0.9, 0.9}, {1, 0, 0})

gr.render(scene_root, 'final_scene.png', 128*4, 128*4,
	  {0, 0, 150}, {0, 0, -800}, {0, 1, 0}, 50,
	  {0.5, 0.5, 0.5}, {white_light})
