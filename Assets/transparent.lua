
newmat2 = gr.new_material({1, 0, 0}, {1, 1, 1}, 25, 0, {0.4, 2})

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25)

mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25)

mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25)

mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25)
wallmat = gr.material({0.5, 0.5, 0.5}, {0,0,0}, 25)
scene_root = gr.node('root')

t1 = gr.cube('mirror')
t1:translate(-0.5,-0.5,0)
t1:scale(100,100,100)
t1:translate(0,0,1000)
scene_root:add_child(t1)
t1:set_material(newmat2)

bg = gr.cube("bg")
bg:translate(-0.5,-0.5,0)
bg:scale(10000,10000,2)
bg:set_material(wallmat)

wall1 = gr.node("wall1")
wall1:add_child(bg)
wall1:translate(0,0,-2000)


wall2 = gr.node("wall2")
wall2:add_child(bg)
wall2:translate(0,0,2000)
wall3 = gr.node("wall3")
wall3:add_child(bg)
wall3:translate(10000,0,0)
wall4 = gr.node("wall4")
wall4:add_child(bg)
wall4:translate(-10000,0,0)
wall5 = gr.node("wall5")
wall5:add_child(bg)
wall5:translate(0,-10000,0)
wall6 = gr.node("wall6")
wall6:add_child(bg)
wall6:translate(0,10000,0)

room = gr.node("room")
room:add_child(wall1)
room:add_child(wall2)
room:add_child(wall3)
room:add_child(wall4)
room:add_child(wall5)
room:add_child(wall6)

--scene_root:add_child(room)

s1 = gr.nh_sphere('s1', {0, 0, -400}, 100)
scene_root:add_child(s1)
s1:set_material(mat1)

s2 = gr.nh_sphere('s2', {200, 50, -100}, 150)
scene_root:add_child(s2)
s2:set_material(mat1)

s3 = gr.nh_sphere('s3', {0, -1200, -500}, 1000)
scene_root:add_child(s3)
s3:set_material(mat2)

b1 = gr.nh_box('b1', {-200, -125, 0}, 100)
scene_root:add_child(b1)
b1:set_material(mat4)

s4 = gr.nh_sphere('s4', {-100, 25, -300}, 50)
scene_root:add_child(s4)
s4:set_material(mat3)

s5 = gr.nh_sphere('s5', {0, 100, -250}, 25)
scene_root:add_child(s5)
s5:set_material(mat1)

-- A small stellated dodecahedron.

steldodec = gr.mesh( 'dodec', 'Assets/smstdodeca.obj' )
steldodec:set_material(mat3)
scene_root:add_child(steldodec)

white_light = gr.light({100,300,0}, {0.9, 0.9, 0.9}, {1, 0, 0})
orange_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})
gr.render(scene_root, 'transparent.png', 512*2, 512*2,
	  {0, 0, 1800}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {white_light, orange_light})
