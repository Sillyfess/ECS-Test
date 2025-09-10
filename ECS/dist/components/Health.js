"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.Health = void 0;
const Component_1 = require("../Component");
class Health extends Component_1.Component {
    constructor(max = 100, current) {
        super();
        this.max = max;
        this.current = current ?? max;
    }
    takeDamage(amount) {
        this.current = Math.max(0, this.current - amount);
    }
    heal(amount) {
        this.current = Math.min(this.max, this.current + amount);
    }
    isDead() {
        return this.current <= 0;
    }
    getPercentage() {
        return (this.current / this.max) * 100;
    }
    clone() {
        return new Health(this.max, this.current);
    }
}
exports.Health = Health;
//# sourceMappingURL=Health.js.map