We are working with [Ambisonic surround sound](https://en.wikipedia.org/wiki/Ambisonics) in this project, and to make things easier we use the Ambisonic Toolkit (ATK) which offers a great number of encoders and decoders.

Check [Introducing the Ambisonic Toolkit](http://www.ambisonictoolkit.net/Help/Guides/Intro-to-the-ATK.html) if you want to get more info.


You have to install [SC plugins](https://github.com/supercollider/sc3-plugins) (which includes ATK) to get the examples working.
Follow installation instructions at their [github repository](https://github.com/supercollider/sc3-plugins)

Make also sure that `MathLib` is installed. You can do so by executing `Quarks.gui` in supercollider IDE.

Here's a diagram of the atk infrastrcuture we'll use:

![lhcvmm Atk Infrastructure](lhcvmm_atk_infrastructure.png)
