let cart = [];
let total = 0;
let allProducts = [];

// Atualiza o relógio
setInterval(() => {
    const now = new Date();
    document.getElementById('clock').textContent = now.toLocaleTimeString();
}, 1000);

// Navegação entre abas
document.getElementById('btn-caixa').onclick = () => showView('caixa');
document.getElementById('btn-estoque').onclick = () => showView('estoque');
document.getElementById('btn-config').onclick = () => showView('config');

function showView(view) {
    // Esconde tudo
    document.getElementById('product-list').style.display = 'none';
    document.getElementById('cart-view').style.display = 'none';
    document.getElementById('inventory-view').style.display = 'none';
    document.getElementById('config-view').style.display = 'none';
    
    // Remove active das nav-items
    document.querySelectorAll('.nav-item').forEach(i => i.classList.remove('active'));

    if (view === 'caixa') {
        document.getElementById('product-list').style.display = 'grid';
        document.getElementById('cart-view').style.display = 'flex';
        document.getElementById('btn-caixa').classList.add('active');
    } else if (view === 'estoque') {
        document.getElementById('inventory-view').style.display = 'block';
        document.getElementById('btn-estoque').classList.add('active');
        renderInventory();
    } else if (view === 'config') {
        document.getElementById('config-view').style.display = 'block';
        document.getElementById('btn-config').classList.add('active');
    }
}

// Carrega produtos do C++ (API Local)
async function loadProducts() {
    try {
        console.log("Tentando buscar produtos de http://localhost:8080/api/produtos...");
        const response = await fetch('http://localhost:8080/api/produtos');
        if (!response.ok) throw new Error("Erro na rede");
        
        allProducts = await response.json();
        console.log("Produtos carregados:", allProducts);
        renderProducts(allProducts);
    } catch (err) {
        console.error("Erro ao carregar produtos:", err);
        const grid = document.getElementById('product-list');
        grid.innerHTML = `<div style="grid-column: 1/-1; text-align: center; color: var(--danger)">
            <h3>⚠️ Erro de Conexão com o C++</h3>
            <p>Certifique-se de que o programa está rodando no terminal (Opção 13).</p>
        </div>`;
    }
}

function renderProducts(products) {
    const grid = document.getElementById('product-list');
    if (!products || products.length === 0) {
        grid.innerHTML = "<p>Nenhum produto encontrado no banco.</p>";
        return;
    }

    grid.innerHTML = products.map(p => `
        <div class="product-card" onclick="addToCart(${p.id}, '${p.nome.replace(/'/g, "\\'")}', ${p.preco})">
            <div class="p-icon">📦</div>
            <div class="p-name">${p.nome}</div>
            <div class="p-price">R$ ${p.preco.toFixed(2)}</div>
            <div style="font-size: 0.7rem; color: var(--text-muted)">Estoque: ${p.estoque}</div>
        </div>
    `).join('');
}

function renderInventory() {
    const tableBody = document.getElementById('inventory-table-body');
    tableBody.innerHTML = allProducts.map(p => `
        <tr>
            <td>${p.id}</td>
            <td>${p.nome}</td>
            <td>R$ ${p.preco.toFixed(2)}</td>
            <td>${p.estoque}</td>
            <td>${p.categoria}</td>
            <td>
                <button class="btn-secondary" onclick="alert('Editar ID ${p.id}')">✏️</button>
            </td>
        </tr>
    `).join('');
}

function addToCart(id, nome, preco) {
    const item = cart.find(i => i.id === id);
    if (item) {
        item.qtd++;
    } else {
        cart.push({ id, nome, preco, qtd: 1 });
    }
    updateCartUI();
}

function updateCartUI() {
    const list = document.getElementById('cart-list');
    const count = document.getElementById('cart-count');
    const totalDisp = document.getElementById('total-venda');
    const subtotalDisp = document.getElementById('subtotal');

    list.innerHTML = cart.map(item => `
        <div class="cart-item">
            <div>
                <strong>${item.qtd}x</strong> ${item.nome}
            </div>
            <div>R$ ${(item.preco * item.qtd).toFixed(2)}</div>
        </div>
    `).join('');

    total = cart.reduce((acc, i) => acc + (i.preco * i.qtd), 0);
    count.textContent = `${cart.reduce((acc, i) => acc + i.qtd, 0)} itens`;
    totalDisp.textContent = `R$ ${total.toFixed(2)}`;
    subtotalDisp.textContent = `R$ ${total.toFixed(2)}`;
}

document.getElementById('finalize-btn').onclick = () => {
    if (cart.length === 0) return alert("Carrinho vazio!");
    document.getElementById('payment-modal').style.display = 'flex';
};

function closeModal() {
    document.getElementById('payment-modal').style.display = 'none';
}

function processPayment() {
    alert("Venda Finalizada com Sucesso!");
    // No futuro, aqui faremos um POST para o C++ processar a venda real
    cart = [];
    updateCartUI();
    closeModal();
}

function openAddProduct() {
    alert("Funcionalidade de Cadastro via Web em desenvolvimento.");
}

// Inicialização
loadProducts();
setInterval(loadProducts, 1000); // Atualiza estoque a cada 1 segundo (Sincronização em tempo real)
