use std::{fs::File, io::{Read, Write}, path::PathBuf};

use clap::Parser;
use rand::random;
use wavefront_obj::obj::{self, TVertex, Vertex};

fn f64todsformat(value: f64) -> u16 {
    ((value * 1024.0) as i16) as u16
}

fn f64totexformat(value: f64, tex_size: f64) -> u16 {
    ((value * 16.0 * tex_size) as i16) as u16
}

fn encode_vtx_16(commands: &mut Vec<u32>, vertex: &Vertex) {
    commands.push(0x23);
    commands.push(((f64todsformat(vertex.x) as u32) << 0x10) + f64todsformat(vertex.y) as u32);
    commands.push(f64todsformat(vertex.z) as u32);
}

fn encode_color(commands: &mut Vec<u32>, color: (f32, f32, f32)) {
    commands.push(0x20);
    let r_part = (color.0 * 31.0) as u32;
    let g_part = (color.1 * 31.0) as u32;
    let b_part = (color.2 * 31.0) as u32;
    commands.push((b_part << 10) + (g_part << 5) + r_part);
}

#[allow(unused)]
fn encode_random_color(commands: &mut Vec<u32>) {
    encode_color(commands, (random(), random(), random()));
}

fn encode_texcoord(commands: &mut Vec<u32>, tex_size: f64, texcoord: &TVertex) {
    commands.push(0x22);
    commands.push(((f64totexformat(1.0 - texcoord.v, tex_size) as u32) << 16) + (f64totexformat(texcoord.u, tex_size) as u32));
}

fn encode_optional_texcoord(commands: &mut Vec<u32>, tex_size: f64, texcoord: Option<&TVertex>) {
    if let Some(texcoord) = texcoord {
        encode_texcoord(commands, tex_size, texcoord);
    }
}

#[derive(Parser, Debug)]
struct Args {
    input: PathBuf,
    output: PathBuf,
}

fn main() {
    let args = Args::parse();
    let mut obj_text = String::new();

    File::open(&args.input)
        .unwrap()
        .read_to_string(&mut obj_text)
        .unwrap();

    let content = obj::parse(obj_text).unwrap();

    let mut tex_size_str = String::new();
    File::open(&args.input.with_extension("texsize"))
        .unwrap()
        .read_to_string(&mut tex_size_str)
        .unwrap();
    let tex_size: f64 = tex_size_str.parse().unwrap();

    let mut output_buffer: Vec<u32> = Vec::new();

    encode_color(&mut output_buffer, (1.0, 1.0, 1.0));

    for object in content.objects {
        for geometry in object.geometry {
            let shapes = geometry.shapes;
            for shape in shapes {
                match shape.primitive {
                    obj::Primitive::Point(_) => panic!(),
                    obj::Primitive::Line(_, _) => panic!(),
                    obj::Primitive::Triangle(v1, v2, v3) => {
                        encode_optional_texcoord(&mut output_buffer, tex_size, v1.1.map(|x| &object.tex_vertices[x]));
                        //encode_random_color(&mut output_buffer);
                        encode_vtx_16(&mut output_buffer, &object.vertices[v1.0]);
                        encode_optional_texcoord(&mut output_buffer, tex_size, v2.1.map(|x| &object.tex_vertices[x]));
                        //encode_random_color(&mut output_buffer);
                        encode_vtx_16(&mut output_buffer, &object.vertices[v2.0]);
                        encode_optional_texcoord(&mut output_buffer, tex_size, v3.1.map(|x| &object.tex_vertices[x]));
                        //encode_random_color(&mut output_buffer);
                        encode_vtx_16(&mut output_buffer, &object.vertices[v3.0]);
                    }
                }
            }
        }
    }

    let mut output_file = File::create(&args.output).unwrap();
    for command in &output_buffer {
        output_file.write(&command.to_le_bytes()).unwrap();
    }    
}
