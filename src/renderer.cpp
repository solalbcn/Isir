#include "renderer.hpp"
#include "integrators/ray_cast_integrator.hpp"
#include "integrators/direct_lighting_integrator.hpp"
#include "integrators/whitted_integrator.hpp"
#include "utils/console_progress_bar.hpp"
#include "utils/random.hpp"

namespace RT_ISICG
{
	Renderer::Renderer() { _integrator = new RayCastIntegrator(); }

	void Renderer::setIntegrator( const IntegratorType p_integratorType )
	{
		if ( _integrator != nullptr ) { delete _integrator; }

		switch ( p_integratorType )
		{
		case IntegratorType::RAY_CAST:
		default:
		{
			_integrator = new RayCastIntegrator();
			break;
		}
		case IntegratorType::DIRECT_LIGHTING:
		{
			_integrator = new DirectLightingIntegrator();
			break;
		}
		case IntegratorType::WHITTED:
		{
			_integrator = new WhittedIntegrator();
			break;
		}
		}
	}

	void Renderer::setBackgroundColor( const Vec3f & p_color )
	{
		if ( _integrator == nullptr ) { std::cout << "[Renderer::setBackgroundColor] Integrator is null" << std::endl; }
		else
		{
			_integrator->setBackgroundColor( p_color );
		}
	}
	void Renderer::setNbLightSamples(const int p_nbLightSamples) {
		if ( _integrator == nullptr ) { std::cout << "[Renderer::setNbLightSamples] Integrator is null" << std::endl; }
		else { _integrator->setNbLightSamples( p_nbLightSamples ); }
	}

	float Renderer::renderImage( const Scene & p_scene, const BaseCamera * p_camera, Texture & p_texture )
	{
		std::cout << (int)_integrator->getType() << std::endl;
		const int width	 = p_texture.getWidth();
		const int height = p_texture.getHeight();
		std::cout << width << std::endl;
		std::cout << height << std::endl;
		Chrono			   chrono;
		ConsoleProgressBar progressBar;
		float p_tmin = 0, p_tmax = 100;
		progressBar.start( height, 50 );
		chrono.start();
		HitRecord p_hitRecord;
		Vec3f color = Vec3f(0.f);
		#pragma omp parallel for
		for ( int j = 0; j < height; j++ )
		{
			for ( int i = 0; i < width; i++ )
			{
				Vec3f colorMean = Vec3f(0);
				for (unsigned int k = 0; k < _nbPixelSamples; k++) {
					float xOffSet = randomFloat();
					float yOffSet = randomFloat();
					float p_sx = ((i + xOffSet) / (width - 1));
					float p_sy = ((j + yOffSet) / (height - 1));
					colorMean += _integrator->Li(p_scene, p_camera->generateRay(p_sx, p_sy), p_tmin, p_tmax);
				}

				colorMean = colorMean / (float)_nbPixelSamples;
				colorMean=glm::clamp(colorMean, 0.f, 1.f);
				p_texture.setPixel(i, j, colorMean);
			}
			progressBar.next();
		}
		// Carr�
		/*
		for ( int j = 0; j < height; j++ )
		{
			for ( int i = 0; i < width; i++ )
			{	
				p_texture.setPixel(i,j,(p_camera->generateRay( (float)(( i + 0.5 ) / ( width - 1 )), (float)( ( j + 0.5 ) / (height - 1) ) ).getDirection()+ 1.f )* 0.5f );
			}
			progressBar.next();
		}
		*/
		

		chrono.stop();
		progressBar.stop();

		return chrono.elapsedTime();
	}
} // namespace RT_ISICG
