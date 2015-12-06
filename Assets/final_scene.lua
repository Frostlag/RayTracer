-- A simple scene with five spheres

uupa_mat = gr.new_material({0,0,0}, {1, 1, 1}, 25, 0.8, {1, 1})
wall_mat = gr.material({1, 1, 1}, {0.5,0.5,0.5}, 10)
floor_mat = gr.material({1, 1, 1}, {0, 0, 0}, 10)
roof_mat = gr.material({1, 1, 1}, {1, 1, 1}, 10)
baoding_mat = gr.new_material({0.0, 0.0, 0.0}, {0.5, 0.7, 0.5}, 25, 0, {0,2})
panel_mat = gr.new_material({0.2, 0.2, 0.2}, {0.5, 0.5, 0.5}, 25, 0.5, {1,10})
table_mat = gr.material({0.3,0.3,0.3},{0.8,0.8,0.8}, 10)
table_leg_mat = gr.new_material({0.2, 0.2, 0.2}, {0.5, 0.5, 0.5}, 25, 0.3, {1,10})
orange = gr.material({0.5,0.5,0.5},{0.5,0.6,0.5}, 0)
monitor_plastic = gr.material({0.1,0.1,0.1},{0.1,0.1,0.1}, 50)

keycap_mat = gr.material({0.8,0.8,0.8},{1,1,1}, 50)
keyboard_mat = gr.material({0.2,0.2,0.2},{1,1,1}, 50)
mouse_mat = gr.material({0.8,0.8,0.8},{1,1,1}, 50)
baoding_holder_mat = gr.material({1, 0, 0}, {0.5, 0.7, 0.5}, 25)
BigSister_tex = gr.texture("Assets/BigSister.png")
Eleanor_mat = gr.material({0,0,0},{1,1,1}, 5)

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25)
mat2 = gr.material({1, 0, 0}, {0.5, 0.7, 0.5}, 25)


scene_root = gr.node('root')

table = gr.node('table')
table_top = gr.cube("table_top")
table_top:translate(-0.5,-0.5,-0.5)
table_top:scale(140,5,80)
table_top:set_material(table_mat)
table:add_child(table_top)

table_leg = gr.cylinder("table_leg")
table_leg:translate(0,-1,0)
table_leg:scale(2,20,2)
table_leg:set_material(table_leg_mat)

table_leg_x_constant = 60
table_leg_y_constant = 30
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
Eleanor_mesh:set_material(Eleanor_mat)
Eleanor_mesh:scale(3,3,3)

Eleanor_base = gr.cylinder("Eleanor_base")
Eleanor_base:scale(10,0.5,10)
Eleanor_base:translate(1,-0.5,1.7)
Eleanor_base:set_material(Eleanor_mat)

Eleanor = gr.node("Eleanor")
Eleanor:add_child(Eleanor_mesh)
Eleanor:add_child(Eleanor_base)
--Eleanor:scale(10,10,10)
--Eleanor:rotate("x",90)

monitor_base = gr.cube("monitor_base")
monitor_base:translate(-0.5,0,-0.5)
monitor_base:scale(70,3,40)
monitor_base:set_material(monitor_plastic)
monitor_column = gr.cube("monitor_column")
monitor_column:translate(-0.5,0,-1)
monitor_column:scale(7,70,7)
monitor_column:set_material(monitor_plastic)
monitor_attacher = gr.cylinder("monitor_attacher")
monitor_attacher:scale(7,5,7)
monitor_attacher:rotate("z",90)
monitor_attacher:translate(0,65,2)
monitor_attacher:set_material(monitor_plastic)
monitor_screen = gr.cube("monitor_screen")
monitor_screen:translate(-0.5,0,-0.5)
monitor_screen:scale(170,100,4)
monitor_screen:translate(0,10,10)
monitor_screen:set_material(monitor_plastic)
monitor_panel = gr.cube("monitor_panel")
monitor_panel:translate(-0.5,0,-0.5)
monitor_panel:scale(160,90,3)
monitor_panel:translate(0,15,11)
monitor_panel:set_material(panel_mat)

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
wall_width = 600
wall_thickness = 5

room = gr.node("room")
room_wall = gr.cube("room_wall")
room_wall:set_material(wall_mat)
room_wall:translate(-0.5,-0.5,-0.5)
room_wall:scale(wall_width,wall_height,wall_thickness)

room_wall1 = gr.node("room_wall1")
room_wall1:add_child(room_wall)
room_wall1:translate(0,0,-wall_width/2)
room:add_child(room_wall1)

window_hole_x_offset = 45
window_hole_y_offset = 45

window_hole1 = gr.cube("window_hole")
window_hole1:translate(-0.5,-0.5,-0.5)
window_hole1:set_material(wall_mat)
window_hole1:scale(40,40,100)
window_hole1:translate(0,0,0)

window_hole2 = gr.cube("window_hole")
window_hole2:translate(-0.5,-0.5,-0.5)
window_hole2:set_material(wall_mat)
window_hole2:scale(40,40,100)
window_hole2:translate(window_hole_x_offset,0,0)

window_hole3 = gr.cube("window_hole")
window_hole3:translate(-0.5,-0.5,-0.5)
window_hole3:set_material(wall_mat)
window_hole3:scale(40,40,100)
window_hole3:translate(0,window_hole_y_offset,0)

window_hole4 = gr.cube("window_hole")
window_hole4:translate(-0.5,-0.5,-0.5)
window_hole4:set_material(wall_mat)
window_hole4:scale(40,40,100)
window_hole4:translate(window_hole_x_offset,window_hole_y_offset,0)


window_holes = gr.node("window_holes")
window_holes:add_child(window_hole1)
window_holes:add_child(window_hole2)
window_holes:add_child(window_hole3)
window_holes:add_child(window_hole4)

--window_hole:rotate("y",90)
--window_holes:set_material(wall_mat)
--room_wall2 = gr.node("room_wall2")
--room_wall2:add_child(window_holes)
--room_wall2:add_child(room_wall)
room_wall21 = gr.subtraction("room_wall21",room_wall,window_hole1)
room_wall22 = gr.subtraction("room_wall22",room_wall21,window_hole2)
room_wall23 = gr.subtraction("room_wall23",room_wall22,window_hole3)
room_wall2 = gr.subtraction("room_wall2",room_wall23,window_hole4)
room_wall2:set_material(wall_mat)

room_wall2:rotate("y",90)
room_wall2:translate(wall_width/2, 0, 0)
room:add_child(room_wall2)

room_wall3 = gr.node("room_wall3")
room_wall3:add_child(room_wall)
room_wall3:rotate("y",90)
room_wall3:translate(-wall_width/2, 0, 0)
room:add_child(room_wall3)

room_wall4 = gr.node("room_wall4")
room_wall4:add_child(room_wall)
room_wall4:translate(0, 0, wall_width/2)
room:add_child(room_wall4)

floor = gr.cube("floor")
floor:translate(-0.5,-0.5,-0.5)
floor:scale(wall_width, wall_thickness, wall_width)
floor:translate(0, -wall_height/2 + 2, 0)
floor:set_material(floor_mat)
room:add_child(floor)

roof = gr.cube("roof")
roof:translate(-0.5,-0.5,-0.5)
roof:scale(wall_width+1, wall_thickness, wall_width+1)
roof:translate(0, wall_height/2 - 2, 0)
roof:set_material(roof_mat)
room:add_child(roof)

baoding_distance = 3
baoding_ball = gr.sphere("baoding_ball")
baoding_ball:scale(3,3,3)
baoding_ball:set_material(baoding_mat)
baoding_ball1 = gr.node("baoding_ball1")
baoding_ball1:translate(baoding_distance,0,0)
baoding_ball1:add_child(baoding_ball)

baoding_ball2 = gr.node("baoding_ball2")
baoding_ball2:translate(-baoding_distance,0,0)
baoding_ball2:add_child(baoding_ball)
baoding_balls = gr.node("baoding_balls")
baoding_holder = gr.torus("baoding_holder",2,6)
baoding_holder:translate(0,-4,0)
baoding_holder:set_material(baoding_holder_mat)
baoding_balls:add_child(baoding_ball1)
baoding_balls:add_child(baoding_ball2)
baoding_balls:add_child(baoding_holder)

keycaps = gr.mesh("keycaps","Assets/keycaps.obj")
keycaps:set_material(keycap_mat)
keycaps:scale(5,5,5)

key_board =gr.cube("key_board")
key_board:set_material(keyboard_mat)
key_board:translate(-0.5,0,-0.5)
key_board:scale(65,2,20)
key_board:translate(10,0,-3)

keyboard = gr.node("keyboard")
keyboard:add_child(keycaps)
keyboard:add_child(key_board)

mouse_head = gr.sphere("mouse_head")
mouse_head:translate(0,0,-1)
mouse_body = gr.cylinder("mouse_body")
mouse_body:scale(1,1,1)
mouse_body:rotate("x",90)
mouse_total = gr.union("mouse_total",mouse_head,mouse_body)
mouse_sub = gr.cube("mouse_sub")
mouse_sub:translate(-0.5,-1,-0.5)
mouse_sub:scale(10,5,10)
mouse = gr.subtraction("mouse",mouse_total, mouse_sub)
mouse:set_material(mouse_mat)

mouse:scale(3,3,3)

mouse:translate(30,2,18)
table:add_child(mouse)

table:add_child(keyboard)
keyboard:rotate("x",10)
keyboard:translate(-30,3,23)
table:add_child(baoding_balls)
--baoding_balls:scale(1.5,1.5,1.5)
baoding_balls:translate(-50,8,-2)

table:add_child(monitor)
monitor:translate(-20,2.3,-20)

table:add_child(uupa)
uupa:rotate("y",-30)
uupa:translate(40,8,-20)


--table:add_child(Eleanor)
Eleanor:scale(1.5,1.5,1.5)
Eleanor:rotate("y",-45)
Eleanor:translate(55,3,0)
table:rotate("y",90)
table:translate(-wall_width/2+50,-35,-wall_width/2+70)
scene_root:add_child(table)

scene_root:add_child(monitor)
scene_root:add_child(room)


white_light = gr.light({5000.0, 100.0, 1500.0}, {0.8, 0.8, 0.8}, {1, 0, 0})
room_light = gr.light({-220,60,-100}, {0.5,0.5,0.5}, {1,0,0})
model_light = gr.light({0,-20,-230}, {0.0,0.0,0.0},{1,0,0})

final_camera_pos = {-100, -10, -160}
final_camera_dir = {-150, -0, -180}

test_camera_pos = {-100, 50, -160}
test_camera_dir = {-150, -0, -180}
gr.render(scene_root, 'final_scene.png', 128*4 ,128*4,  final_camera_pos, final_camera_dir, {0, 1, 0}, 50,  {0.3, 0.3, 0.3}, {white_light, room_light})
--gr.render(scene_root, 'final_scene.png', 128*4, 128*4,  test_camera_pos, test_camera_dir, {0, 1, 0}, 50,  {0.5, 0.5, 0.5}, {white_light, room_light})
