<template>
    <div class="product-details">
      <div v-if="product" class="product-card">
        <div class="product-image-container">
          <img 
            :src="product.img_url
              ? `http://localhost:3000/uploads/${product.img_url.split('/').pop()}` 
              : 'https://via.placeholder.com/200'"
            alt="商品图片"
            class="product-image" />
        </div>
        <div class="product-info">
          <h1 class="product-title">{{ product.title }}</h1>
          <p class="product-price"><strong>价格:</strong> {{ product.price }} 元</p>
          <p class="product-category"><strong>类别:</strong> {{ product.category }}</p>
          <p class="product-desc"><strong>描述:</strong> {{ product.desc }}</p>
          <div class="action-buttons">
            <button @click="addToFavorites" class="btn btn-favorites">添加到收藏夹</button>
            <button @click="contactSeller" class="btn btn-contact">联系卖家</button>
          </div>
        </div>
      </div>
      <div v-else>
        <p>商品加载中...</p>
      </div>
    </div>
  </template>
  
  <script>
  export default {
    name: 'ProductDetails',
    data() {
      return {
        product: JSON.parse(this.$route.query.product),
      };
    },
    methods: {
      addToFavorites() {
        // 添加到收藏夹逻辑
      },
      contactSeller() {
        // 联系卖家逻辑
      },
    },
    mounted() {
      console.log('Product details:', this.product);
    },
  };
  </script>
  
  <style scoped>
  .product-details {
    padding: 3rem;
    background-color: #f1f1f1; /* 背景色改为柔和的灰色 */
    min-height: 100vh;
  }
  
  .product-card {
    display: flex;
    background-color: #fff; /* 卡片背景色为白色 */
    border-radius: 12px;
    box-shadow: 0 8px 24px rgba(0, 0, 0, 0.15); /* 增加阴影效果 */
    overflow: hidden;
    max-width: 1200px;
    margin: auto;
    padding: 2rem;
  }
  
  .product-image-container {
    flex: 1;
    max-width: 400px;
    margin-right: 2rem;
  }
  
  .product-image {
    width: 100%;
    height: auto;
    object-fit: cover;
    border-radius: 12px;
    box-shadow: 0 4px 12px rgba(0, 0, 0, 0.1);
  }
  
  .product-info {
    flex: 2;
  }
  
  .product-title {
    font-size: 2rem;
    font-weight: bold;
    color: #34495e; /* 深灰蓝色 */
    margin-bottom: 1rem;
  }
  
  .product-price,
  .product-category,
  .product-desc {
    font-size: 1.2rem;
    color: #7f8c8d; /* 暖灰色 */
    margin-bottom: 1rem;
  }
  
  .action-buttons {
    display: flex;
    gap: 1rem;
  }
  
  .btn {
    padding: 0.8rem 1.6rem;
    border: none;
    border-radius: 8px;
    cursor: pointer;
    transition: background-color 0.3s ease;
    font-size: 1rem;
  }
  
  .btn-favorites {
    background-color: #f39c12; /* 温暖的橙色 */
    color: #fff;
  }
  
  .btn-favorites:hover {
    background-color: #e67e22; /* 橙色的深色 */
  }
  
  .btn-contact {
    background-color: #2ecc71; /* 亮绿色 */
    color: #fff;
  }
  
  .btn-contact:hover {
    background-color: #27ae60; /* 绿色的深色 */
  }
  
  @media (max-width: 768px) {
    .product-card {
      flex-direction: column;
    }
  
    .product-image-container {
      max-width: 100%;
      margin-bottom: 2rem;
    }
  
    .product-info {
      text-align: center;
    }
  
    .action-buttons {
      flex-direction: column;
    }
  }
  </style>
  