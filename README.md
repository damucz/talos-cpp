# Introduction

TalosVFX is node based, open source VFX Editor with powerfull interface. You can check the [website](https://talosvfx.com/), [github](https://github.com/rockbite/talos) (Java runtimes included), [discord](https://discord.gg/zjEGT5zEzM) or [tutorials](https://www.youtube.com/channel/UCyOHcyqkNolfn2fkAZhQD_Q).

TalosVFX C++ runtimes allows you to load the effects exported from the editor and run the emitters in your engine. Because of these runtimes are engine independent, you are required to implement at least renderes (drawables) for the engine of your choice. The sources for our [custom engine](https://github.com/damucz/talos-cpp/tree/master/engine-sample) are provided for your convenience. You can also implement your own loading, but there is already RapidJson loader implemented (using FILE internally).

The API is very rough at the time. Please be patient, ask by direct message, github issue or discord. I'm happy to help, refactor the API or just explain my ideas.

# Usage

1. Implement your engine classes for loading and rendering.
2. Add the content of [source](https://github.com/damucz/talos-cpp/tree/master/loaders) folder to your project.

You can use TalosVFX runtimes in any C++ engine by implementing the few classes.
- `IFactory` to create your custom classes for the drawables and loader
- `IFileProvider` to load the data files ([RapidJson loader provided](https://github.com/damucz/talos-cpp/tree/master/loaders))
- `IParticleDrawable` for few drawable classes, at least one, depending on your effect needs

## `Talos::IFactory`
In factory you are responsible for creating classes to be used by Talos, usually with your custom implementation.

### Loader is mandatory:
1. `Talos::IFileProvider* CreateFileProvider(const char* fileName)`
Should create class inherited from IFileProvider, responsible for loading json data files and supply the data on demand (explained later).<br />
You can you use provided RapidJson loader just by: `return new Talos::RapidJsonProvider(fileName);`

### The drawables are all optional (by default they are empty), you should implement at least one of them to see anything:
1. TextureRegionModule/SpriteModule: `Talos::TextureRegionDrawable* CreateTextureRegionDrawable(const char* region)`<br />
This is basic drawable for the most effects. Just takes Particle and draw it using its values.
2. PolylineModule: `Talos::PolylineDrawable* CreatePolylineDrawable(const char* region)`<br />
This drawable should be able to draw the lines from given points.
3. ShadedSpriteModule: `Talos::ShadedDrawable* CreateShadedDrawable(const char* shdrFileName)`<br />
This drawable is intended to use a special shader to draw. The shader can be exported from TalosVFX editor. You are on your own to load it and use it.

Other drawables are not implemented at the time, but should be easy to add an interface at least.

## `IFileProvider`
You can inherit IFileProvider to supply your own effect loader. It should be able to return the Emitters with its Modules and Connections.
See the RapidJsonProvider, which uses RapidJson lib to load the json and return the data to the Talos, when the effect needs to be loaded.

## `IParticleDrawable`
When rendering the effect you should go through the emitters (`for (auto* particleEmitter : _particleEffectInstance->GetEmitters())`)
and their active particles (`for (auto* particle : particleEmitter->GetActiveParticles())`) to render them.<br />
Use:<br />
`particleEmitter->IsVisible()` - when false, you can skip the emitter.<br />
`particleEmitter->IsBlendAdd()` and `particleEmitter->IsAdditive()` to determine blending type.<br />
You should be able to get all the information from the `particle`, including the drawable object, which should be your own class.<br />
Every drawable is rendered using `void Draw(RenderContext* context, Particle* particle, float r, float g, float b, float a)` method.<br />
If you need to pass any engine-specific data (renderer, matrix, scale) into the drawable classes, please inherit RenderContext (simple data class, empty by default) and pass it to the Draw. There you can cast it and read.<br />
Every particle could have its own drawable type, which was created by your factory. By default, the Draw method is empty. You should implement the drawables you need.<br />

Note: For convenience I'm sharing all the inherited classes for our engine. Not directly usable for you, but you can go and see the idea. Approach in your engine should be similar.

# NOT implemented modules

* VectorFieldModule

* FlipbookModule
* NinePatchModule
* RibbonModule

* RandomInputModule
* ScriptModule

# ToDo

* pool allocators - Particle (EmitterInstance), Polyline
* correct effect scaling

# License

Code released under [the MIT license](https://github.com/damucz/talos-cpp/blob/master/LICENSE).
