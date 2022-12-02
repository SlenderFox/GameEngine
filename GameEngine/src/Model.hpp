#pragma region
#pragma once
#include "Mesh.hpp"

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;
enum aiTextureType: int;
#pragma endregion

namespace Engine
{
	struct Model
	{
		friend class Renderer;
	private:
		bool m_loadTextures;

		std::unique_ptr<std::vector<std::unique_ptr<Mesh>>> m_meshes = nullptr;
		std::vector<Texture*> m_textures;
		std::string m_directory;

		Shader *m_shader;
		Camera *m_cameraRef;

		inline void LoadModel(const std::string *inPath) noexcept;

		inline void ProcessNode(
			const aiNode *inNode,
			const aiScene *inScene
		) noexcept;

		_NODISCARD inline
		std::unique_ptr<Mesh> ProcessMesh(
			const aiMesh *inMesh,
			const aiScene *inScene
		) noexcept;

		_NODISCARD inline
		std::vector<Texture*> LoadMaterialTextures(
			const aiMaterial *inMat,
			const aiTextureType inType,
			const Texture::TexType inTexType
		) const noexcept;

		inline void LoadTexturesToShader() const noexcept;

	public:
		Model(
			const std::string *inModelPath,
			const std::string *inShaderPath,
			Camera *inCamera = nullptr,
			const bool inLoadTextures = true
		) noexcept;

		~Model();

		void Draw(const Camera *inCamera = nullptr) const noexcept;

		constexpr void SetCameraRef(Camera *inCamera) noexcept;
		constexpr void SetShaderRef(Shader *inShader) noexcept;

		_NODISCARD Shader *GetShaderRef() const noexcept;
		_NODISCARD Mesh *GetMeshAt(const uint16_t inPos) const noexcept;
	};
}
