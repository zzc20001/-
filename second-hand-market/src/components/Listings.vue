<template>
  <div class="product-page">
    <header class="header">
      <h2>商品管理</h2>
      <button class="publish-btn" @click="openModal">+ 发布商品</button>
    </header>

    <!-- 商品列表 -->
    <div class="product-list" v-if="products.length > 0">
      <h3>已发布商品</h3>
      <div class="card-container">
        <!-- 遍历每个商品 -->
        <div 
          class="product-card" 
          v-for="product in products" 
          :key="product.id"
        >
          <h4>{{ product.name }}</h4>
          <p class="price">￥{{ product.price.toFixed(2) }}</p>
          <img 
            :src="product.image_path 
              ? `http://localhost:3000/uploads/${product.image_path.split('/').pop()}` 
              : 'https://via.placeholder.com/200'" 
            alt="商品图片" 
          />
          <button class="edit-btn" @click="openEditModal(product)">编辑</button>
        </div>
      </div>
    </div>

    <!-- 显示暂无商品 -->
    <div class="no-products" v-else>
      <img src="https://via.placeholder.com/200" alt="暂无商品" />
      <p>暂无商品，点击上方按钮发布您的第一个商品吧！</p>
    </div>

    <!-- 添加商品模态框 -->
    <div v-if="showModal" class="modal">
      <div class="modal-content">
        <button class="close-btn" @click="closeModal">×</button>
        <h3>添加商品</h3>
        <form @submit.prevent="saveProduct">
          <div class="form-group">
            <label for="name">商品名称：</label>
            <input id="name" v-model="newProduct.name" placeholder="请输入商品名称" required />
          </div>

          <div class="form-group">
            <label for="price">价格：</label>
            <input id="price" type="number" v-model.number="newProduct.price" placeholder="请输入价格" required />
          </div>

          <div class="form-group">
            <label for="description">商品描述：</label>
            <textarea id="description" v-model="newProduct.description" placeholder="请输入商品描述" required></textarea>
          </div>

          <div class="form-group">
            <label for="category">商品类型：</label>
            <input id="category" v-model="newProduct.category" placeholder="请输入商品类型" required />
          </div>

          <div class="form-group">
            <label for="image">商品图片：</label>
            <input type="file" id="image" @change="handleImageUpload" accept="image/*" />
            <div v-if="newProduct.imageUrl" class="image-preview">
              <img :src="newProduct.imageUrl" alt="商品图片预览" />
            </div>
          </div>

          <div class="modal-actions">
            <button type="submit" class="save-btn">保存</button>
            <button type="button" class="cancel-btn" @click="closeModal">取消</button>
          </div>
        </form>
      </div>
    </div>
  </div>
</template>

<script>
import api from "../api";

export default {
  name: "ListingsPage",
  data() {
  return {
      products: [], // 存储商品列表
      showModal: false,
      isEditing: false, // 新增一个 isEditing 属性
      currentProductId: 0, // 初始化 currentProductId
      newProduct: {
        name: "",
        price: 0,
        description: "",
        category: "",
        image: null,
        imageUrl: "",
      },
    };
  },
  mounted() {
    this.loadProducts();
  },
  methods: {
        // 打开模态框
    openModal() {
      this.showModal = true;
    },
    openEditModal(product) {
      this.isEditing = true; // 设置为编辑模式
      this.currentProductId = product.product_id; // 保存当前商品ID
      this.newProduct = { 
        ...product, // 直接填充商品数据
        imageUrl: product.image_path ? `http://localhost:3000/uploads/${product.image_path.split('/').pop()}` : ''
      };
      this.showModal = true;  // 显示模态框
    },
    // 加载商品列表
    loadProducts() {
      const token = sessionStorage.getItem("token");
      api
        .get("/get-products", { headers: { Authorization: `Bearer ${token}` } })
        .then((response) => {
          this.products = response.data.data; // 更新产品数据
          console.log("加载商品列表成功:", this.products);
        })
        .catch((error) => {
          console.error("加载商品列表失败:", error);
        });
    },
    // 关闭模态框
    closeModal() {
      this.showModal = false;
      this.isEditing = false;
      this.newProduct = {
        name: "",
        price: 0,
        description: "",
        category: "",
        image: null,
        imageUrl: "",
      };
    },
    // 处理图片上传
    handleImageUpload(event) {
      const file = event.target.files[0];
      if (file && file.type.startsWith("image/")) {
        const reader = new FileReader();
        reader.onload = (e) => {
          this.newProduct.imageUrl = e.target.result;
        };
        reader.readAsDataURL(file);
        this.newProduct.image = file;
      } else {
        alert("请选择有效的图片文件！");
      }
    },
    // 保存商品
    saveProduct() {
  // 上传时必须确保所有字段都已填写
  if (!this.isEditing) {
    if (
      this.newProduct.image && 
      this.newProduct.name && 
      this.newProduct.price > 0 && 
      this.newProduct.description
    ) {
      const formData = new FormData();
      formData.append("name", this.newProduct.name);
      formData.append("description", this.newProduct.description);
      formData.append("price", this.newProduct.price);
      formData.append("category", this.newProduct.category);
      formData.append("image", this.newProduct.image);

      const token = sessionStorage.getItem("token");

      // 发送上传请求
      this.isLoading = true;
      api.post("/upload-product", formData, {
        headers: { Authorization: `Bearer ${token}` },
      })
      .then(() => {
        alert("商品上传成功！");
        this.closeModal();
        this.loadProducts(); // 重新加载商品列表
      })
      .catch((error) => {
        console.error("上传失败:", error);
        alert("商品上传失败，请重试！");
      })
      .finally(() => {
        this.isLoading = false;
      });

    } else {
      alert("请确保所有字段都已填写完整，且图片已选择！");
    }

  } else {
    // 更新时，提交修改的字段
    if (!this.currentProductId) {
   console.error("Product ID is undefined or invalid");
   alert("商品ID无效，请选择一个商品。");
   return;  // 阻止发送请求
}
    const formData = new FormData();

    // 仅当字段修改时才提交
    if (this.newProduct.name) formData.append("name", this.newProduct.name);
    if (this.newProduct.price > 0) formData.append("price", this.newProduct.price);
    if (this.newProduct.description) formData.append("description", this.newProduct.description);
    if (this.newProduct.category) formData.append("category", this.newProduct.category);
    if (this.newProduct.image) formData.append("image", this.newProduct.image);

    const token = sessionStorage.getItem("token");

    // 发送更新请求
    this.isLoading = true;
    api.put(`/update-product/${this.currentProductId}`, formData, {
      headers: { Authorization: `Bearer ${token}` },
    })
    .then(() => {
      alert("商品更新成功！");
      this.closeModal();
      this.loadProducts(); // 重新加载商品列表
    })
    .catch((error) => {
      console.error("更新失败:", error);
      alert("商品更新失败，请重试！");
    })
    .finally(() => {
      this.isLoading = false;
    });
  }
    },
  },
};
</script>

<style scoped>
/* 原页面样式 */
.product-page {
  max-width: 900px;
  margin: 40px auto;
  padding: 20px;
  background: linear-gradient(145deg, #ffffff, #fffae3);
  border-radius: 20px;
  box-shadow: 0 8px 24px rgba(0, 0, 0, 0.2);
  font-family: 'Poppins', sans-serif;
  color: #2e2e2e;
  
  /* 添加滚动功能 */
  max-height: 80vh; /* 设置最大高度 */
  overflow-y: auto; /* 启用垂直滚动条 */
}


.header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 20px;
}

.publish-btn {
  padding: 12px 24px;
  background: linear-gradient(90deg, #ff7e5f, #feb47b);
  color: #fff;
  border: none;
  border-radius: 30px;
  font-size: 16px;
  font-weight: 600;
  cursor: pointer;
  transition: transform 0.2s, box-shadow 0.2s;
  box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
}

.card-container {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(240px, 1fr));
  gap: 20px;
  max-height: 600px;  /* 设定容器的最大高度 */
  overflow-y: auto;  /* 启用垂直滚动条 */
}


.product-card {
  background: #fff;
  border-radius: 15px;
  padding: 20px;
  box-shadow: 0 6px 16px rgba(0, 0, 0, 0.1);
  text-align: center;
  transition: transform 0.3s, box-shadow 0.3s;
}

.product-card:hover {
  transform: translateY(-10px);
  box-shadow: 0 10px 20px rgba(0, 0, 0, 0.2);
}

.product-card img {
  width: 100%;
  height: 120px;
  object-fit: cover;
  border-radius: 10px;
  margin: 10px 0;
}

.edit-btn {
  padding: 10px 16px;
  background: linear-gradient(90deg, #4facfe, #00f2fe);
  color: #fff;
  border: none;
  border-radius: 20px;
  font-size: 14px;
  cursor: pointer;
}

/* 模态框样式 */
.modal {
  position: fixed;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  background: rgba(0, 0, 0, 0.5);
  display: flex;
  justify-content: center;
  align-items: center;
  z-index: 1000;
  animation: fadeIn 0.3s ease-in-out;
}

/* 模态框内容 */
.modal-content {
  position: relative;
  background: linear-gradient(135deg, #e0f7fa, #b3e5fc); /* 蓝白色渐变 */
  border-radius: 20px;
  padding: 30px;
  width: 90%;
  max-width: 400px;
  box-shadow: 0 10px 30px rgba(0, 0, 0, 0.3);
  animation: zoomIn 0.3s ease-in-out;
  overflow: hidden;
}

/* 关闭按钮 */
.close-btn {
  position: absolute;
  top: 10px;
  right: 10px;
  background: none;
  border: none;
  font-size: 20px;
  font-weight: bold;
  color: #aaa;
  cursor: pointer;
  transition: color 0.3s ease;
}

.close-btn:hover {
  color: #ff5f57;
}

/* 表单标题 */
.modal-content {
  position: relative;
  background: linear-gradient(135deg, #e0f7fa, #b3e5fc); /* 蓝白色渐变 */
  border-radius: 20px;
  padding: 30px;
  width: 90%;
  max-width: 400px;
  box-shadow: 0 10px 30px rgba(0, 0, 0, 0.3);
  animation: zoomIn 0.3s ease-in-out;
  overflow-y: auto;  /* 启用垂直滚动 */
  max-height: 80vh;  /* 设置最大高度 */
}


/* 表单组样式 */
.form-group {
  margin-bottom: 20px;
}

.form-group label {
  display: block;
  margin-bottom: 5px;
  font-size: 14px;
  color: #666;
}

.form-group input,
.form-group textarea {
  width: 100%;
  padding: 12px 15px;
  border: 1px solid #ddd;
  border-radius: 10px;
  font-size: 16px;
  background-color: #fafafa;
  transition: all 0.3s ease;
  box-sizing: border-box; /* 确保文本框不会超出边界 */
}

.form-group input:focus,
.form-group textarea:focus {
  border-color: #2196f3;
  box-shadow: 0 0 8px rgba(33, 150, 243, 0.3);
  outline: none;
}

/* 图片上传预览 */
.image-preview {
  margin-top: 10px;
  text-align: center;
}

.image-preview img {
  max-width: 100%;
  height: auto;
  border-radius: 8px;
  box-shadow: 0 4px 10px rgba(0, 0, 0, 0.1);
}

/* 模态框按钮 */
.modal-actions {
  display: flex;
  gap: 10px;
  justify-content: space-between;
  margin-top: 20px;
}

.save-btn,
.cancel-btn {
  flex: 1;
  padding: 10px 15px;
  font-size: 16px;
  border: none;
  border-radius: 20px;
  font-weight: 600;
  cursor: pointer;
  transition: transform 0.3s ease, box-shadow 0.3s ease;
}

.save-btn {
  background: linear-gradient(90deg, #FFB74D, #FF8A65);
  color: white;
}

.save-btn:hover {
  transform: translateY(-2px);
  box-shadow: 0 6px 20px rgba(76, 175, 80, 0.3);
}

.cancel-btn {
  background: linear-gradient(90deg, #FFB74D, #FF8A65);
  color: white;
}

.cancel-btn:hover {
  transform: translateY(-2px);
  box-shadow: 0 6px 20px rgba(244, 67, 54, 0.3);
}

/* 动画效果 */
@keyframes fadeIn {
  from {
    opacity: 0;
  }
  to {
    opacity: 1;
  }
}

@keyframes zoomIn {
  from {
    transform: scale(0.8);
    opacity: 0;
  }
  to {
    transform: scale(1);
    opacity: 1;
  }
}
</style>
