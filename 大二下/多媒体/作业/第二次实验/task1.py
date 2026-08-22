import cv2
import numpy as np

def create_cat_collage():
    img1 = cv2.imread('cat_1.png')
    img2 = cv2.imread('cat_2.png')
    img3 = cv2.imread('cat_3.png')
    img4 = cv2.imread('cat_4.png')

    images = [img1, img2, img3, img4]

    top_row = np.hstack((img1, img2))
    bottom_row = np.hstack((img3, img4))
    collage = np.vstack((top_row, bottom_row))

    cv2.imwrite('cat_collage_result.png', collage)
    print("拼贴画已保存为: cat_collage_result.png")
    
    cv2.imshow('Cat Collage', collage)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

if __name__ == "__main__":
    create_cat_collage()