-- A simple scene with five spheres
grey = 0.6
uupa_mat = gr.new_material({grey,grey,grey}, {1, 1, 1}, 25, 0, {1, 1})
wall_mat = gr.material({0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, 10)
floor_mat = gr.material({0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, 25)
mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25)
mat2 = gr.material({1, 0, 0}, {0.5, 0.7, 0.5}, 25)

BigSister_tex = gr.texture("Assets/BigSister.png")
Eleanor = gr.mesh("Eleanor","Assets/Eleanor.obj")

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


uupa_base = gr.sphere("uupa_base")
uupa_base:scale(5,5,5)
uupa_ring = gr.torus("uupa_ring",4,100)
uupa_ring:scale(0.05,0.05,0.05)
uupa_body = gr.subtraction("uupa_body",uupa_base,uupa_ring)
uupa_body:set_material(uupa_mat)

uupa_eye_base = gr.sphere("uupa_eye_base")
uupa_eye_base:scale(0.5,0.5,0.5)
uupa_eye_dimple = gr.sphere("uupa_eye_dimple")
uupa_eye_dimple:scale(0.45,0.45,0.3)
uupa_eye_dimple:translate(0,0,0.2)
uupa_eye = gr.subtraction("uupa_eye",uupa_eye_base,uupa_eye_dimple)
uupa_eye:set_material(uupa_mat)
--uupa_eye:translate(0,0,5)


uupa_eye_rotate_x_amount = 40
uupa_eye_rotate_y_amount = 25

uupa_eye1 = gr.node("uupa_eye1")
uupa_eye1:add_child(uupa_eye)
uupa_eye1:translate(0,0,5)
uupa_eye1:rotate("y",uupa_eye_rotate_y_amount)
uupa_eye1:rotate("x",-uupa_eye_rotate_x_amount)


uupa_eye2 = gr.node("uupa_eye2")
uupa_eye2:add_child(uupa_eye)
uupa_eye2:translate(0,0,5)
uupa_eye2:rotate("y",-uupa_eye_rotate_y_amount)
uupa_eye2:rotate("x",-uupa_eye_rotate_x_amount)

uupa_leg = gr.sphere("uupa_leg")
uupa_leg:set_material(uupa_mat)
uupa_leg:scale(0.5,2,0.5)
uupa_leg:translate(0,-5,0)

uupa_leg_rotate_x_amount = 40
uupa_leg_rotate_y_amount = 40

uupa_leg1 = gr.node("uupa_leg1")
uupa_leg1:add_child(uupa_leg)
uupa_leg1:rotate("x",uupa_leg_rotate_x_amount)
uupa_leg1:rotate("y",uupa_leg_rotate_y_amount)

uupa_leg2 = gr.node("uupa_leg2")
uupa_leg2:add_child(uupa_leg)
uupa_leg2:rotate("x",uupa_leg_rotate_x_amount)
uupa_leg2:rotate("y",-uupa_leg_rotate_y_amount)

uupa_leg3 = gr.node("uupa_leg3")
uupa_leg3:add_child(uupa_leg)
uupa_leg3:rotate("x",-uupa_leg_rotate_x_amount)
uupa_leg3:rotate("y",uupa_leg_rotate_y_amount)

uupa_leg4 = gr.node("uupa_leg4")
uupa_leg4:add_child(uupa_leg)
uupa_leg4:rotate("x",-uupa_leg_rotate_x_amount)
uupa_leg4:rotate("y",-uupa_leg_rotate_y_amount)

uupa_legs = gr.node("uupa_legs")
uupa_legs:add_child(uupa_leg1)
uupa_legs:add_child(uupa_leg2)
uupa_legs:add_child(uupa_leg3)
uupa_legs:add_child(uupa_leg4)

uupa_ear_rotate_z_amount = 40

uupa_ear_base = gr.sphere("uupa_ear_base")
uupa_ear_base:scale(1,1,3)
uupa_ear_base:translate(0,0,0)
uupa_ear_dimple = gr.cylinder("uupa_ear_dimple")
uupa_ear_dimple:scale(10,10,4)
uupa_ear_dimple:translate(0,0,5)

uupa_ear = gr.subtraction("uupa_concave_ear",uupa_ear_base, uupa_ear_dimple)
uupa_ear:set_material(uupa_mat)
uupa_ear:translate(0,5,2)
uupa_ear:rotate("x",-20)

uupa_ear1 = gr.node("uupa_ear1")
uupa_ear1:add_child(uupa_ear)
uupa_ear1:rotate("z",uupa_ear_rotate_z_amount)

uupa_ear2 = gr.node("uupa_ear2")
uupa_ear2:add_child(uupa_ear)
uupa_ear2:rotate("z",-uupa_ear_rotate_z_amount)

uupa_ears = gr.node("uupa_ears")
uupa_ears:add_child(uupa_ear1)
uupa_ears:add_child(uupa_ear2)

uupa_nose = gr.sphere("uupa_nose")
uupa_nose:scale(0.5,0.3,0.3)
uupa_nose:translate(0,2.7,4)
uupa_nose:set_material(uupa_mat)

uupa_smile1 = gr.cylinder("uupa_smile1")
uupa_smile1:scale(0.2,1,0.2)
uupa_smile1:rotate("z",-60)
uupa_smile1:translate(2.5,0,0)
uupa_smile2 = gr.cylinder("uupa_smile2")
uupa_smile2:scale(0.2,1,0.2)
uupa_smile2:rotate("z",60)
uupa_smile2:translate(0.8,0,0)
uupa_smile3 = gr.union("uupa_smile3",uupa_smile1,uupa_smile2)
uupa_smile3:scale(0.5,0.5,1)
uupa_smile4 = gr.union("uupa_smile4",uupa_smile1,uupa_smile2)
uupa_smile4:scale(-0.5,0.5,1)

uupa_smile = gr.union("uupa_smile",uupa_smile3,uupa_smile4)
uupa_smile:translate(0,0,4.9)
uupa_smile:rotate("x",-15)
uupa_smile:set_material(uupa_mat)

uupa = gr.node("uupa")
uupa:add_child(uupa_body)
uupa:add_child(uupa_eye1)
uupa:add_child(uupa_eye2)
uupa:add_child(uupa_legs)
uupa:add_child(uupa_ears)
uupa:add_child(uupa_nose)
uupa:add_child(uupa_smile)
--uupa:scale(10,10,10)
--uupa:rotate("x",40)
--uupa:rotate("y",90)

Eleanor_tex = gr.texture("Assets/BigSister.png")
Eleanor_mesh = gr.mesh("Eleanor_mesh","Assets/Eleanor.obj")
Eleanor_mesh:set_texture(Eleanor_tex)
Eleanor_mesh:set_material(mat1)
Eleanor_mesh:scale(3,3,3)

Eleanor_base = gr.cylinder("Eleanor_base")
Eleanor_base:scale(8,0.5,8)
Eleanor_base:translate(1,-0.5,1.7)
Eleanor_base:set_material(mat1)

Eleanor = gr.node("Eleanor")
Eleanor:add_child(Eleanor_mesh)
Eleanor:add_child(Eleanor_base)
Eleanor:scale(10,10,10)
--Eleanor:rotate("x",90)

monitor_base = gr.cube("monitor_base")
monitor_base:translate(-0.5,0,-0.5)
monitor_base:scale(70,3,40)
monitor_base:set_material(mat1)
monitor_column = gr.cube("monitor_column")
monitor_column:translate(-0.5,0,-1)
monitor_column:scale(7,70,7)
monitor_column:set_material(mat1)
monitor_attacher = gr.cylinder("monitor_attacher")
monitor_attacher:scale(7,5,7)
monitor_attacher:rotate("z",90)
monitor_attacher:translate(0,65,2)
monitor_attacher:set_material(mat1)
monitor_screen = gr.cube("monitor_screen")
monitor_screen:translate(-0.5,0,-0.5)
monitor_screen:scale(170,100,4)
monitor_screen:translate(0,10,10)
monitor_screen:set_material(mat1)
monitor_panel = gr.cube("monitor_panel")
monitor_panel:translate(-0.5,0,-0.5)
monitor_panel:scale(160,90,3)
monitor_panel:translate(0,15,11)
monitor_panel:set_material(mat2)

monitor = gr.node("monitor")
monitor:add_child(monitor_base)
monitor:add_child(monitor_column)
monitor:add_child(monitor_attacher)
monitor:add_child(monitor_screen)
monitor:add_child(monitor_panel)

monitor:scale(0.5,0.5,0.5)
--monitor:rotate("y",45)
--monitor:rotate("y",90)

wall_height = 150
wall_width = 300
wall_thickness = 5

room = gr.node("room")
room_wall = gr.cube("room_wall")
room_wall:set_material(wall_mat)
room_wall:translate(-0.5,-0.5,-0.5)
room_wall:scale(wall_width,wall_height,wall_thickness)

room_wall1 = gr.node("room_wall1")
room_wall1:add_child(room_wall)
room:add_child(room_wall1)

window_hole_x_offset = 50
window_hole_y_offset = 50

window_hole = gr.cube("window_hole")
window_hole:translate(-0.5,-0.5,-0.5)
window_hole:scale(40,40,100)
window_hole1 = gr.node("window_hole1")
window_hole1:add_child(window_hole)
window_hole2 = gr.node("window_hole2")
window_hole2:translate(window_hole_x_offset,0,0)
window_hole2:add_child(window_hole)
window_hole3 = gr.node("window_hole3")
window_hole3:add_child(window_hole)
window_hole3:translate(0,window_hole_y_offset,0)
window_hole4 = gr.node("window_hole4")
window_hole4:add_child(window_hole)
window_hole4:translate(window_hole_x_offset,window_hole_y_offset,0)

window_holes = gr.node("window_holes")
window_holes:add_child(window_hole1)
window_holes:add_child(window_hole2)
window_holes:add_child(window_hole3)
window_holes:add_child(window_hole4)

--window_hole:rotate("y",90)
--window_hole:set_material(wall_mat)
--room_wall2 = gr.node("room_wall2")
--room_wall2:add_child(window_hole)
room_wall2 = gr.union("room_wall2",window_holes,window_holes)
room_wall2:set_material(wall_mat)
--room_wall2:add_child(room_wall)
room_wall2:rotate("y",90)
room_wall2:translate(wall_width/2, 0, wall_width/2)
room:add_child(room_wall2)

room_wall3 = gr.node("room_wall3")
room_wall3:add_child(room_wall)
room_wall3:rotate("y",90)
room_wall3:translate(-wall_width/2, 0, wall_width/2)
room:add_child(room_wall3)

room_wall4 = gr.node("room_wall4")
room_wall4:add_child(room_wall)
room_wall4:translate(0, 0, wall_width)
room:add_child(room_wall4)

floor = gr.cube("floor")
floor:translate(-0.5,-0.5,-0.5)
floor:scale(wall_width, wall_thickness, wall_width)
floor:translate(0, -wall_height/2, wall_width/2)
floor:set_material(floor_mat)
room:add_child(floor)

scene_root:add_child(table)
--scene_root:add_child(uupa)
--scene_root:add_child(Eleanor)
--scene_root:add_child(monitor)
scene_root:add_child(room)


white_light = gr.light({300.0, 100.0, 0.0}, {0.9, 0.9, 0.9}, {1, 0, 0})

gr.render(scene_root, 'final_scene.png', 128*4, 128*4,
	  {0, 500, 10}, {0, 0, 0}, {0, 1, 0}, 50,
	  {0.5, 0.5, 0.5}, {white_light})
