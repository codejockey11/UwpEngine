#include "pch.h"

#include "Common/CImageManager.h"

UwpEngine::CImageManager::CImageManager()
{
	memset(this, 0x00, sizeof(CImageManager));
}

UwpEngine::CImageManager::CImageManager(CVideoDevice* d)
{
	memset(this, 0x00, sizeof(CImageManager));

	m_videoDevice = d;

	m_images = new CLinkList();
}

UwpEngine::CImageManager::~CImageManager()
{
	CLinkListNode* images = m_images->m_list;

	while (images)
	{
		CImage* image = (CImage*)images->m_element;

		delete image;

		images = images->m_next;
	}

	delete m_images;
}

UwpEngine::CImage* UwpEngine::CImageManager::MakeImage(const char* imagename)
{
	CImage* image = GetImage(imagename);
	
	if (image)
	{
		return image;
	}

	image = new CImage(m_videoDevice, imagename);

	if (image->m_isInitialized)
	{
		m_images->Add(image, imagename);

		return image;
	}

	throw Platform::FailureException::CreateException(E_FAIL);

	delete image;

	return nullptr;
}

void UwpEngine::CImageManager::DeleteImage(const char* imagename)
{
	CLinkListNode* image = m_images->Search(imagename);

	if (image)
	{
		m_images->Delete(image);
	}
}

UwpEngine::CImage* UwpEngine::CImageManager::GetImage(const char* imagename)
{
	CLinkListNode* image = m_images->Search(imagename);

	if (image)
	{
		return (CImage*)image->m_element;
	}

	return nullptr;
}