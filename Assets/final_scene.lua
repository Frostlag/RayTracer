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

scene_root:add_child(table)



white_light = gr.light({100.0, 0.0, 300.0}, {0.9, 0.9, 0.9}, {1, 0, 0})

gr.render(scene_root, 'final_scene.png', 128*4, 128*4,
	  {0, 0, 200}, {0, 0, -800}, {0, 1, 0}, 50,
	  {0.5, 0.5, 0.5}, {white_light})
