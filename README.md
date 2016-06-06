# Tiny 3D engine on stm32 ( maple mini board )

It's tiny 3D engine use scaneline algorithm, this way is capalble to calcualte flat shading, lighting and z buffer algorithm too, using 
only 4 * 320 byte as a buffer ( line buffer).

It can render full 320x240 pixel 16 bit deep frame, with using stm32 mcu which has only 20 KBram.


Now, it uses float point numbers, in the feauture I plan to use fix point numbers. In this case it could be a little bit faster.


![Alt text](/images/img.jpg?raw=true "Tiny 3D engine")
