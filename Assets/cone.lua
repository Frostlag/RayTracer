-- A simple scene with five spheres

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25)

scene_root = gr.node('root')

s1 = gr.cube('cylinder')
s1:scale(100,100,100)
s1:translate(-20,-10,-10)
--scene_root:add_child(s1)
s1:set_material(mat1)

s2 = gr.cube('cylinder')
s2:scale(50,500,50)
s2:translate(20,-10,-10)
--scene_root:add_child(s2)
s2:set_material(mat1)

u1 = gr.conjunction('conjunction',s1,s2)
u1:rotate("x",45)
u1:set_material(mat2)
scene_root:add_child(u1)


white_light = gr.light({-100.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
orange_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(scene_root, 'cone.png', 1024, 1024,
	  {0, 0, 800}, {0, 0, -800}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {white_light, orange_light})
